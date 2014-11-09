#include "lyxSocketAddress.h"
#include "lyxNumberFormatter.h"
#include "lyxHostEntry.h"
#include "lyxDNS.h"

namespace lyx {

SocketAddress::SocketAddress() {
    newIPv4();
}

SocketAddress::SocketAddress(const IPAddress& hostAddress, uint16_t portNumber) {
    init(hostAddress, portNumber);
}

SocketAddress::SocketAddress(uint16_t portNumber) {
    init(IPAddress(), portNumber);
}

SocketAddress::SocketAddress(const std::string& hostAddress, const std::string& service) {
    init(hostAddress, resolveService(service));
}

SocketAddress::SocketAddress(const std::string& hostAddress, uint16_t portNumber) {
    init(hostAddress, portNumber);
}

SocketAddress::SocketAddress(const SocketAddress& socketAddress) {
    newIPv4(reinterpret_cast<const sockaddr_in*>(socketAddress.addr()));
}

SocketAddress::SocketAddress(const struct sockaddr* sockAddr, socklen_t length) {
    if (length == sizeof(struct sockaddr_in))
        newIPv4(reinterpret_cast<const struct sockaddr_in*>(sockAddr));
    newIPv4();
}

SocketAddress::~SocketAddress() {
}

SocketAddress& SocketAddress::operator = (const SocketAddress& socketAddress) {
    if (&socketAddress != this) {
        if (socketAddress.family() == IPAddress::IPv4) {
            newIPv4(reinterpret_cast<const sockaddr_in*>(socketAddress.addr()));
        }
        else {
            // TODO: newIPv6();
        }
    }
    return *this;
}

IPAddress SocketAddress::host() const {
    return pImpl()->host();
}

uint16_t SocketAddress::port() const {
    return ntohs(pImpl()->port());
}

const struct sockaddr* SocketAddress::addr() const {
    return pImpl()->addr();
}

int SocketAddress::af() const {
    return pImpl()->af();
}

std::string SocketAddress::toString() const {
    std::string result;
    result.append(host().toString());
    result.append(":");
    NumberFormatter::append(result, port());
    return result;
}

int SocketAddress::length() const {
    return pImpl()->length();
}

void SocketAddress::init(const IPAddress& hostAddress, uint16_t portNumber) {
    newIPv4(hostAddress, portNumber);
}

void SocketAddress::init(const std::string& hostAddress, uint16_t portNumber) {
    IPAddress ip;
    if (IPAddress::tryParse(hostAddress, ip)) {
        init(ip, portNumber);
    }
    else {
        HostEntry he = DNS::hostByName(hostAddress);
        HostEntry::AddressList addresses = he.addresses();
        if (addresses.size() > 0) {
            init(addresses[0], portNumber);
        }
        else {
            // default ip
            init(ip, portNumber);
        }
    }
}

uint16_t SocketAddress::resolveService(const std::string& service) {
    struct servent* se = getservbyname(service.c_str(), NULL);
    return ntohs(se->s_port);
}

} // namespace lyx
