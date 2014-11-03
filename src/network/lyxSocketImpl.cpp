#include "lyxSocketImpl.h"
#include <cassert>

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

void SocketImpl::connect(const SocketAddress& address) {
    if (_sockfd == -1) {
        init(address.af());
    }
    int rc;
    rc = TEMP_FAILURE_RETRY(::connect(_sockfd, address.addr(), address.length()));
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

void SocketImpl::init(int af) {
    initSocket(af, SOCK_STREAM);
}

void SocketImpl::initSocket(int af, int type, int proto) {
    assert(_sockfd == -1);

    _sockfd = ::socket(af, type, proto);
}

} // namespace lyx
