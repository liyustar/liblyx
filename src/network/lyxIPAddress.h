#ifndef LIBLYX_NETWORK_LYXIPADDRESS_H_
#define LIBLYX_NETWORK_LYXIPADDRESS_H_

#include <memory>
#include <vector>
#include "lyxIPAddressImpl.h"

namespace lyx {

class IPAddress {
    public:
        typedef std::vector<IPAddress> List;

        enum Family {
            IPv4,
            IPv6
        };

        IPAddress();
        IPAddress(const IPAddress& addr);
        explicit IPAddress(Family family);
        IPAddress(const std::string& addr, Family family);
        IPAddress(const struct sockaddr& sockaddr);

        ~IPAddress();

        std::string toString() const;
        bool isWildcard() const;
        bool isBroadcast() const;
        bool isLoopback() const;

        bool operator == (const IPAddress& addr) const;
        bool operator != (const IPAddress& addr) const;

        socklen_t length() const;

        const void* addr() const;

        int af() const;
        IPAddress parse(const std::string& addr);
        bool tryParse(const std::string& addr, IPAddress& result);

    private:
        typedef IPAddressImpl Impl;
        typedef std::shared_ptr<Impl> Ptr;

        Ptr pImpl() const;

        void newIPv4(const void* hostAddr);

        void newIPv4();

        Ptr _pImpl;
};

inline IPAddress::Ptr IPAddress::pImpl() const {
    return _pImpl;
}

inline void IPAddress::newIPv4(const void* hostAddr) {
    _pImpl.reset(new IPv4AddressImpl(hostAddr));
}

inline void IPAddress::newIPv4() {
    _pImpl.reset(new IPv4AddressImpl());
}

} // namespace lyx

#endif // LIBLYX_NETWORK_LYXIPADDRESS_H_
