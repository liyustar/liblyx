#include "lyxStreamSocketImpl.h"

namespace lyx {

StreamSocketImpl::StreamSocketImpl() {
}

StreamSocketImpl::StreamSocketImpl(IPAddress::Family family) {
    if (family == IPAddress::IPv4) {
        init(AF_INET);
    }
    init(AF_INET);
}

StreamSocketImpl::StreamSocketImpl(int sockfd) :
    SocketImpl(sockfd)
{
}

StreamSocketImpl::~StreamSocketImpl() {
}

int StreamSocketImpl::sendBytes(const void* buffer, int length, int flags) {
    const char* p = reinterpret_cast<const char*>(buffer);
    int remaining = length;
    int sent = 0;
    while (remaining > 0) {
        int n = SocketImpl::sendBytes(p, remaining, flags);
        p += n;
        sent += n;
        remaining -= n;
    }
    return sent;
}

} // namespace lyx
