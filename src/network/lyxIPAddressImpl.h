#ifndef LIBLYX_NETWORK_LYXIPADDRESSIMPL_H_
#define LIBLYX_NETWORK_LYXIPADDRESSIMPL_H_

#include <string>
#include "lyxSocketDefs.h"

namespace lyx {

class IPAddressImpl {
    public:
        enum Family {
            IPv4,
            IPv6
        };

        virtual ~IPAddressImpl();

        virtual std::string toString() const = 0;
        virtual socklen_t length() const = 0;
        virtual const void* addr() const = 0;
        virtual Family family() const = 0;
        virtual int af() const = 0;
        virtual bool isWildcard() const = 0;
        virtual bool isBroadcast() const = 0;
        virtual bool isLoopback() const = 0;

    protected:
        IPAddressImpl();

    private:
        IPAddressImpl(const IPAddressImpl&);
        IPAddressImpl& operator = (const IPAddressImpl&);
};

class IPv4AddressImpl : public IPAddressImpl {
    public:
        IPv4AddressImpl();
        IPv4AddressImpl(const void* addr);
        IPv4AddressImpl(const IPv4AddressImpl& addr);
        IPv4AddressImpl& operator = (const IPv4AddressImpl&);
        std::string toString() const;
        socklen_t length() const;
        const void* addr() const;
        Family family() const;
        int af() const;
        bool isWildcard() const;
        bool isBroadcast() const;
        bool isLoopback() const;
        static IPv4AddressImpl parse(const std::string& addr);

    private:
        struct in_addr _addr;
};

} // namespace lyx

#endif // LIBLYX_NETWORK_LYXIPADDRESSIMPL_H_
