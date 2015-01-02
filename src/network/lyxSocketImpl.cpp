#include "lyxSocketImpl.h"
#include "lyxStreamSocketImpl.h"
#include "lyxNetException.h"
#include "lyxBugcheck.h"
#include <cstring>
#include <sys/epoll.h>
#include <unistd.h>
#include <stropts.h>
#include <fcntl.h>

namespace lyx {

SocketImpl::SocketImpl():
    _sockfd(-1),
    _blocking(true)
{
}

SocketImpl::SocketImpl(int sockfd):
    _sockfd(sockfd),
    _blocking(true)
{
}

SocketImpl::~SocketImpl() {
    close();
}

SocketImpl* SocketImpl::acceptConnection(SocketAddress& clientAddr) {
    if (_sockfd == -1) {
        return NULL;
    }
    char buffer[SocketAddress::MAX_ADDRESS_LENGTH];
    struct sockaddr* pSA = reinterpret_cast<struct sockaddr*>(buffer);
    socklen_t saLen = sizeof(buffer);
    int sd;
    sd = TEMP_FAILURE_RETRY(::accept(_sockfd, pSA, &saLen));
    if (sd != -1) {
        clientAddr = SocketAddress(pSA, saLen);
        return new StreamSocketImpl(sd);
    }
    return NULL;
}

void SocketImpl::connect(const SocketAddress& address) {
    if (_sockfd == -1) {
        init(address.af());
    }
    int rc;
    rc = TEMP_FAILURE_RETRY(::connect(_sockfd, address.addr(), address.length()));
    if (rc != 0) {
        int err = lastError();
        error(err, address.toString());
    }
}

void SocketImpl::connect(const SocketAddress& address, const Timespan& timeout) {
    if (_sockfd == -1) {
        init(address.af());
    }

    setBlocking(false);

    try {
        int rc = ::connect(_sockfd, (sockaddr*) address.addr(), address.length());
        if (rc != 0) {
            int err = lastError();
            if (err != EINPROGRESS && err != EWOULDBLOCK)
                error(err, address.toString());
            if (!poll(timeout, SELECT_READ | SELECT_WRITE | SELECT_ERROR))
                throw TimeoutException("connect timed out", address.toString());
            err = socketError();
            if (err != 0) error(err);
        }
    }
    catch (Exception&) {
        setBlocking(true);
        throw;
    }

    setBlocking(true);
}

void SocketImpl::bind(const SocketAddress& address, bool reuseAddress) {
    if (_sockfd == -1) {
        init(address.af());
    }
    if (reuseAddress) {
        setReuseAddress(true);
        setReusePort(true);
    }
    int rc = ::bind(_sockfd, address.addr(), address.length());
    if (rc != 0) error(address.toString());
}

void SocketImpl::listen(int backlog) {
    if (_sockfd == -1) {
        throw InvalidSocketException();
    }
    int rc = ::listen(_sockfd, backlog);
    if (rc != 0) error();
}

void SocketImpl::close() {
    if (_sockfd != -1) {
        ::close(_sockfd);
        _sockfd = -1;
    }
}

void SocketImpl::shutdownReceive() {
    if (_sockfd == -1) throw InvalidSocketException();

    int rc = ::shutdown(_sockfd, 0);
    if (rc != 0) error();
}

void SocketImpl::shutdownSend() {
    if (_sockfd == -1) throw InvalidSocketException();

    int rc = ::shutdown(_sockfd, 1);
    if (rc != 0) error();
}

void SocketImpl::shutdown() {
    if (_sockfd == -1) throw InvalidSocketException();

    int rc = ::shutdown(_sockfd, 2);
    if (rc != 0) error();
}

int SocketImpl::sendBytes(const void* buffer, int length, int flags) {
    if (_sockfd == -1) throw InvalidSocketException();
    int rc;
    rc = TEMP_FAILURE_RETRY(::send(_sockfd, buffer, length, flags));
    return rc;
}

int SocketImpl::receiveBytes(void* buffer, int length, int flags) {
    if (_sockfd == -1) throw InvalidSocketException();
    int rc;
    rc = TEMP_FAILURE_RETRY(::recv(_sockfd, buffer, length, flags));
    if (rc < 0) {
        int err = lastError();
        if (err == EAGAIN && !_blocking)
            ;
        else if (err == EAGAIN || err == ETIMEDOUT)
            throw TimeoutException(err);
        else
            error(err);
    }
    return rc;
}

bool SocketImpl::poll(const Timespan& timeout, int mode) {
    int sockfd = _sockfd;
    if (sockfd == -1) throw InvalidSocketException();

    int epollfd = epoll_create(1);
    if (epollfd < 0) {
        char buf[1024];
        strerror_r(errno, buf, sizeof(buf));
        error(std::string("Can't create epoll queue: ") + buf);
    }

    struct epoll_event evin;
    std::memset(&evin, 0, sizeof(evin));

    if (mode & SELECT_READ)
        evin.events |= EPOLLIN;
    if (mode & SELECT_WRITE)
        evin.events |= EPOLLOUT;
    if (mode & SELECT_ERROR)
        evin.events |= EPOLLERR;

    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &evin) < 0) {
        char buf[1024];
        strerror_r(errno, buf, sizeof(buf));
        ::close(epollfd);
        error(std::string("Can't insert socket to epoll queue: ") + buf);
    }

    Timespan remainingTime(timeout);
    int rc;
    do {
        struct epoll_event evout;
        std::memset(&evout, 0, sizeof(evout));

        Timestamp start;
        rc = epoll_wait(epollfd, &evout, 1, remainingTime.totalMilliseconds());
        if (rc < 0 && lastError() == EINTR) {
            Timestamp end;
            Timespan waited = end - start;
            if (waited < remainingTime)
                remainingTime -= waited;
            else
                remainingTime = 0;
        }
    } while (rc < 0 && lastError() == EINTR);

    ::close(epollfd);
    if (rc < 0) error();
    return rc > 0;
}

void SocketImpl::setSendTimeout(const Timespan& timeout) {
    setOption(SOL_SOCKET, SO_SNDTIMEO, timeout);
}

Timespan SocketImpl::getSendTimeout() {
    Timespan result;
    getOption(SOL_SOCKET, SO_SNDTIMEO, result);
    return result;
}

void SocketImpl::setReceiveTimeout(const Timespan& timeout) {
    setOption(SOL_SOCKET, SO_RCVTIMEO, timeout);
}

Timespan SocketImpl::getReceiveTimeout() {
    Timespan result;
    getOption(SOL_SOCKET, SO_RCVTIMEO, result);
    return result;
}

SocketAddress SocketImpl::address() const {
    if (_sockfd == -1) {
        return SocketAddress();
    }

    char buffer[SocketAddress::MAX_ADDRESS_LENGTH];
    struct sockaddr* pSA = reinterpret_cast<struct sockaddr*>(buffer);
    socklen_t saLen = sizeof(buffer);
    int rc = ::getsockname(_sockfd, pSA, &saLen);
    if (rc == 0) {
        return SocketAddress(pSA, saLen);
    }
    else {
        return SocketAddress();
    }
}

SocketAddress SocketImpl::peerAddress() const {
    if (_sockfd == -1) {
        return SocketAddress();
    }

    char buffer[SocketAddress::MAX_ADDRESS_LENGTH];
    struct sockaddr* pSA = reinterpret_cast<struct sockaddr*>(buffer);
    socklen_t saLen = sizeof(buffer);
    int rc = ::getpeername(_sockfd, pSA, &saLen);
    if (rc == 0) {
        return SocketAddress(pSA, saLen);
    }
    else {
        return SocketAddress();
    }
}

void SocketImpl::setReuseAddress(bool flag) {
    int value = flag ? 1 : 0;
    setOption(SOL_SOCKET, SO_REUSEADDR, value);
}

bool SocketImpl::getReuseAddress() {
    int value(0);
    getOption(SOL_SOCKET, SO_REUSEADDR, value);
    return value != 0;
}

void SocketImpl::setReusePort(bool flag) {
    int value = flag ? 1 : 0;
    setOption(SOL_SOCKET, SO_REUSEPORT, value);
}

bool SocketImpl::getReusePort() {
    int value(0);
    getOption(SOL_SOCKET, SO_REUSEPORT, value);
    return value != 0;

}

void SocketImpl::setBlocking(bool flag) {
    int arg = fcntl(F_GETFL);
    long flags = arg & ~O_NONBLOCK;
    if (!flag) flags |= O_NONBLOCK;
    (void) fcntl(F_SETFL, flags);

    _blocking = flag;
}

int SocketImpl::socketError() {
    int result(0);
    getOption(SOL_SOCKET, SO_ERROR, result);
    return result;
}

void SocketImpl::init(int af) {
    initSocket(af, SOCK_STREAM);
}

void SocketImpl::initSocket(int af, int type, int proto) {
    lyx_assert(_sockfd == -1);

    _sockfd = ::socket(af, type, proto);
}

void SocketImpl::setOption(int level, int option, const Timespan& value) {
    struct timeval tv;
    tv.tv_sec   = (long) value.totalSeconds();
    tv.tv_usec  = (long) value.useconds();

    setRawOption(level, option, &tv, sizeof(tv));
}

void SocketImpl::setRawOption(int level, int option, const void* value, socklen_t length) {
    if (_sockfd == -1) throw InvalidSocketException();

    int rc = ::setsockopt(_sockfd, level, option, reinterpret_cast<const char*>(value), length);
    if (rc == -1) error();
}

void SocketImpl::getOption(int level, int option, int& value) {
    socklen_t len = sizeof(value);
    getRawOption(level, option, &value, len);
}

void SocketImpl::getOption(int level, int option, Timespan& value) {
    struct timeval tv;
    socklen_t len = sizeof(tv);
    getRawOption(level, option, &tv, len);
    value.assign(tv.tv_sec, tv.tv_usec);
}

void SocketImpl::getRawOption(int level, int option, void* value, socklen_t& length) {
    if (_sockfd == -1) throw InvalidSocketException();

    int rc = ::getsockopt(_sockfd, level, option, reinterpret_cast<char*>(value), &length);
    if (rc == -1) error();
}

int SocketImpl::fcntl(int request) {
    int rc = ::fcntl(_sockfd, request);
    if (rc == -1) error();
    return rc;
}

int SocketImpl::fcntl(int request, long arg) {
    int rc = ::fcntl(_sockfd, request, arg);
    if (rc == -1) error();
    return rc;
}

void SocketImpl::error() {
    int err = lastError();
    std::string empty;
    error(err, empty);
}

void SocketImpl::error(int code) {
    std::string empty;
    error(code, empty);
}

void SocketImpl::error(const std::string& arg) {
    error(lastError(), arg);
}

void SocketImpl::error(int code, const std::string& arg) {
    switch (code) {
        default:
            IOException(arg, code);
    }
}

} // namespace lyx
