#include "lyxSocketImpl.h"

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

} // namespace lyx
