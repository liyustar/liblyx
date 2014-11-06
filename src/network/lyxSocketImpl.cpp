#include "lyxSocketImpl.h"
#include <cassert>
#include "lyxStreamSocketImpl.h"

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
}

void SocketImpl::bind(const SocketAddress& address, bool reuseAddress) {
    if (_sockfd == -1) {
        init(address.af());
    }
    int rc = ::bind(_sockfd, address.addr(), address.length());
}

void SocketImpl::listen(int backlog) {
    if (_sockfd == -1) {
        return;
    }
    int rc = ::listen(_sockfd, backlog);
}

void SocketImpl::close() {
    if (_sockfd != -1) {
        ::close(_sockfd);
        _sockfd = -1;
    }
}

void SocketImpl::shutdownReceive() {
    int rc = ::shutdown(_sockfd, 0);
}

void SocketImpl::shutdownSend() {
    int rc = ::shutdown(_sockfd, 1);
}

void SocketImpl::shutdown() {
    int rc = ::shutdown(_sockfd, 2);
}

int SocketImpl::sendBytes(const void* buffer, int length, int flags) {
    if (_sockfd == -1) {
        return -1;
    }
    int rc;
    rc = TEMP_FAILURE_RETRY(::send(_sockfd, buffer, length, flags));
    return rc;
}

int SocketImpl::receiveBytes(void* buffer, int length, int flags) {
    if (_sockfd == -1) {
        return -1;
    }
    int rc;
    rc = TEMP_FAILURE_RETRY(::recv(_sockfd, buffer, length, flags));
    return rc;
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

void SocketImpl::init(int af) {
    initSocket(af, SOCK_STREAM);
}

void SocketImpl::initSocket(int af, int type, int proto) {
    assert(_sockfd == -1);

    _sockfd = ::socket(af, type, proto);
}

} // namespace lyx
