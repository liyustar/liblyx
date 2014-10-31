#include "lyxIPAddressImpl.h"
#include <cstring>
#include <string>
#include "lyxSocketDefs.h"
#include "lyxNumberFormatter.h"

namespace lyx {

IPAddressImpl::IPAddressImpl() {
}

IPAddressImpl::~IPAddressImpl() {
}

IPv4AddressImpl::IPv4AddressImpl() {
    std::memset(&_addr, 0, sizeof(_addr));
}

IPv4AddressImpl::IPv4AddressImpl(const void* addr) {
    std::memcpy(&_addr, addr, sizeof(_addr));
}

IPv4AddressImpl::IPv4AddressImpl(const IPv4AddressImpl& addr) {
    std::memcpy(&_addr, &addr._addr, sizeof(_addr));
}

IPv4AddressImpl& IPv4AddressImpl::operator = (const IPv4AddressImpl& addr) {
    std::memcpy(&_addr, &addr._addr, sizeof(_addr));
    return *this;
}

std::string IPv4AddressImpl::toString() const {
    const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&_addr);
    std::string result;
    result.reserve(16);
    NumberFormatter::append(result, bytes[0]);
    result.append(".");
    NumberFormatter::append(result, bytes[1]);
    result.append(".");
    NumberFormatter::append(result, bytes[2]);
    result.append(".");
    NumberFormatter::append(result, bytes[3]);
    return result;
}

socklen_t IPv4AddressImpl::length() const {
    return sizeof(_addr);
}

const void* IPv4AddressImpl::addr() const {
    return &_addr;
}

IPv4AddressImpl::Family IPv4AddressImpl::family() const {
    return IPv4AddressImpl::IPv4;
}

int IPv4AddressImpl::af() const {
    return AF_INET;
}

bool IPv4AddressImpl::isWildcard() const {
    return _addr.s_addr == INADDR_ANY;
}

bool IPv4AddressImpl::isBroadcast() const {
    return _addr.s_addr == INADDR_NONE;
}

bool IPv4AddressImpl::isLoopback() const {
    return (ntohl(_addr.s_addr) & 0xFF000000) == 0x7F000000; // 127.0.0.1 to 127.255.255.255
}

IPv4AddressImpl IPv4AddressImpl::parse(const std::string& addr) {
    if (addr.empty()) return IPv4AddressImpl();
    struct in_addr ia;
    if (inet_aton(addr.c_str(), &ia))
        return IPv4AddressImpl(&ia);
    else
        return IPv4AddressImpl();
}

bool IPv4AddressImpl::operator == (const IPv4AddressImpl& addr) const {
    return 0 == std::memcmp(&_addr, &addr._addr, sizeof(_addr));
}

bool IPv4AddressImpl::operator != (const IPv4AddressImpl& addr) const {
    return !(*this == addr);
}

} // namespace lyx
