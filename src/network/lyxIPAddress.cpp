#include "lyxIPAddress.h"
#include <cstring>

namespace lyx {

IPAddress::IPAddress() {
    newIPv4();
}

IPAddress::IPAddress(const IPAddress& addr) {
    newIPv4(addr.addr());
}

IPAddress::IPAddress(Family family) {
    newIPv4();
    // TODO: if not IPV4, throw Exception
}

IPAddress::IPAddress(const std::string& addr) {
    IPv4AddressImpl addr4(IPv4AddressImpl::parse(addr));
    newIPv4(addr4.addr());
}

IPAddress::IPAddress(const std::string& addr, Family family) {
    IPv4AddressImpl addr4(IPv4AddressImpl::parse(addr));
    newIPv4(addr4.addr());
}

IPAddress::IPAddress(const void* addr, socklen_t length) {
    if (length == sizeof(struct in_addr)) {
        newIPv4(addr);
    }
}

IPAddress::IPAddress(const struct sockaddr& sockaddr) {
    // unsigned short fanily = sockaddr.sa_family;
    newIPv4(&reinterpret_cast<const struct sockaddr_in*>(&sockaddr)->sin_addr);
}

IPAddress::~IPAddress() {
}

std::string IPAddress::toString() const {
    return pImpl()->toString();
}

bool IPAddress::isWildcard() const {
    return pImpl()->isWildcard();
}

bool IPAddress::isBroadcast() const {
    return pImpl()->isBroadcast();
}

bool IPAddress::isLoopback() const {
    return pImpl()->isLoopback();
}

bool IPAddress::operator == (const IPAddress& a) const {
    socklen_t l1 = length();
    socklen_t l2 = a.length();
    if (l1 == l2)
        return std::memcmp(addr(), a.addr(), l1) == 0;
    else
        return false;
}

bool IPAddress::operator != (const IPAddress& a) const {
    socklen_t l1 = length();
    socklen_t l2 = a.length();
    if (l1 == l2)
        return std::memcmp(addr(), a.addr(), l1) != 0;
    else
        return true;
}

socklen_t IPAddress::length() const {
    return pImpl()->length();
}

const void* IPAddress::addr() const {
    return pImpl()->addr();
}

IPAddress::Family IPAddress::family() const {
    return static_cast<IPAddress::Family>(pImpl()->family());
}

int IPAddress::af() const {
    return pImpl()->af();
}

IPAddress IPAddress::parse(const std::string& addr) {
    return IPAddress(addr, IPv4);
}

bool IPAddress::tryParse(const std::string& addr, IPAddress& result) {
    IPv4AddressImpl impl4(IPv4AddressImpl::parse(addr));
    // if (impl4 != IPv4AddressImpl()) {
        result.newIPv4(impl4.addr());
        return true;
    // }
    return false;
}


} // namespace lyx
