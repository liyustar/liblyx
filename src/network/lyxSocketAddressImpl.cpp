#include "lyxSocketAddressImpl.h"
#include <cstring>

namespace lyx {

SocketAddressImpl::SocketAddressImpl() {
}

SocketAddressImpl::~SocketAddressImpl() {
}

IPv4SocketAddressImpl::IPv4SocketAddressImpl() {
    std::memset(&_addr, 0, sizeof(_addr));
    _addr.sin_family = AF_INET;
}

IPv4SocketAddressImpl::IPv4SocketAddressImpl(const struct sockaddr_in* addr) {
    std::memcpy(&_addr, addr, sizeof(_addr));
}

IPv4SocketAddressImpl::IPv4SocketAddressImpl(const void* addr, uint16_t port) {
    std::memset(&_addr, 0, sizeof(_addr));
    _addr.sin_family = AF_INET;
    std::memcpy(&_addr.sin_addr, addr, sizeof(_addr.sin_addr));
    _addr.sin_port = port;
}

bool IPv4SocketAddressImpl::operator == (const IPv4SocketAddressImpl& addr) const {
    return 0 == std::memcmp(&_addr, &addr._addr, sizeof(_addr));
}

bool IPv4SocketAddressImpl::operator != (const IPv4SocketAddressImpl& addr) const {
    return !(*this == addr);
}

} // namespace lyx
