#ifndef LIBLYX_NETWORK_LYXSOCKETADDRESS_H_
#define LIBLYX_NETWORK_LYXSOCKETADDRESS_H_

#include "lyxSocketAddressImpl.h"

namespace lyx {

class SocketAddress {
    public:
        SocketAddress();
        SocketAddress(const IPAddress& hostAddress, uint16_t portNumber);
        SocketAddress(uint16_t portNumber);
        SocketAddress(const std::string& hostAddress, uint16_t portNumber);
        SocketAddress(const SocketAddress& socketAddress);
        SocketAddress(const struct sockaddr* sockAddr, socklen_t length);

        ~SocketAddress();

        SocketAddress& operator = (const SocketAddress& socketAddress);

        IPAddress host() const;
        uint16_t port() const;
        const struct sockaddr* addr() const;
        int af() const;
        std::string toString() const;

        enum {
            MAX_ADDRESS_LENGTH = sizeof(struct sockaddr_in6)
        };

    protected:
        void init(const IPAddress& hostAddress, uint16_t portNumber);
        void init(const std::string& hostAddress, uint16_t portNumber);
        uint16_t resolveService(const std::string& service);

    private:
        typedef SocketAddressImpl Impl;
        typedef std::shared_ptr<Impl> Ptr;

        Ptr pImpl() const;

        void newIPv4();
        void newIPv4(const sockaddr_in*);
        void newIPv4(const IPAddress& hostAddress, uint16_t portNumber);

        Ptr _pImpl;
};

inline SocketAddress::Ptr SocketAddress::pImpl() const {
    return _pImpl;
}

inline void SocketAddress::newIPv4() {
    _pImpl.reset(new IPv4SocketAddressImpl());
}

inline void SocketAddress::newIPv4(const sockaddr_in* sockAddr) {
    _pImpl.reset(new IPv4SocketAddressImpl(sockAddr));
}

inline void SocketAddress::newIPv4(const IPAddress& hostAddress, uint16_t portNumber) {
    _pImpl.reset(new IPv4SocketAddressImpl(hostAddress.addr(), htons(portNumber)));
}


} // namespace lyx

#endif // LIBLYX_NETWORK_LYXSOCKETADDRESS_H_
