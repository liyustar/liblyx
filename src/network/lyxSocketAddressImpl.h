#ifndef LIBLYX_NETWORK_LYXSOCKETADDRESSIMPL_H_
#define LIBLYX_NETWORK_LYXSOCKETADDRESSIMPL_H_

#include "lyxSocketDefs.h"
#include "lyxIPAddress.h"

namespace lyx {

class SocketAddressImpl {
    public:
        virtual ~SocketAddressImpl();

        virtual IPAddress host() const = 0;
        virtual uint16_t port() const = 0;
        virtual socklen_t length() const = 0;
        virtual const struct sockaddr* addr() const = 0;
        virtual int af() const = 0;

    protected:
        SocketAddressImpl();

    private:
        SocketAddressImpl(const SocketAddressImpl&);
        SocketAddressImpl& operator = (const SocketAddressImpl&);
};

class IPv4SocketAddressImpl : public SocketAddressImpl {
    public:
        IPv4SocketAddressImpl();
        IPv4SocketAddressImpl(const struct sockaddr_in* addr);
        IPv4SocketAddressImpl(const void* addr, uint16_t port);

        IPAddress host() const;
        uint16_t port() const;
        socklen_t length() const;
        const struct sockaddr* addr() const;
        int af() const;

        bool operator == (const IPv4SocketAddressImpl& addr) const;
        bool operator != (const IPv4SocketAddressImpl& addr) const;

    private:
        struct sockaddr_in _addr;
};

inline IPAddress IPv4SocketAddressImpl::host() const {
    return IPAddress(&_addr.sin_addr, sizeof(_addr.sin_addr));
}

inline uint16_t IPv4SocketAddressImpl::port() const {
    return _addr.sin_port;
}

inline socklen_t IPv4SocketAddressImpl::length() const {
    return sizeof(_addr);
}

inline const struct sockaddr* IPv4SocketAddressImpl::addr() const {
    return reinterpret_cast<const struct sockaddr*>(&_addr);
}

inline int IPv4SocketAddressImpl::af() const {
    return _addr.sin_family;
}

} // namespace lyx

#endif // LIBLYX_NETWORK_LYXSOCKETADDRESSIMPL_H_
