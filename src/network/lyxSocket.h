#ifndef LIBLYX_NETWORK_LYXSOCKET_H_
#define LIBLYX_NETWORK_LYXSOCKET_H_

#include "lyxSocketImpl.h"
#include "lyxTimespan.h"
#include <vector>
#include <memory>

namespace lyx {

class Socket {
    public:
        enum SelectMode {
            SELECT_READ  = 1,
            SELECT_WRITE = 2,
            SELECT_ERROR = 4
        };

        typedef std::vector<Socket> SocketList;
        typedef std::shared_ptr<SocketImpl> Ptr;

        Socket();
        Socket(const Socket& socket);

        Socket& operator = (const Socket& socket);
        virtual ~Socket();

        void close();

        static int select(SocketList& readList, SocketList& writeList, SocketList& exceptList, const Timespan& timeout);

        bool poll(const Timespan& timeout, int mode) const;

        void setSendTimeout(const Timespan& timeout);
        Timespan getSendTimeout();
        void setReceiveTimeout(const Timespan& timeout);
        Timespan getReceiveTimeout();

        SocketAddress address() const;
        SocketAddress peerAddress() const;

        Ptr impl() const;

        void init(int af);

    protected:
        Socket(SocketImpl* pImpl);
        int sockfd() const;

    private:
        Ptr _pImpl;
};

inline void Socket::close() {
    _pImpl->close();
}

inline Socket::Ptr Socket::impl() const {
    return _pImpl;
}

inline int Socket::sockfd() const {
    return _pImpl->sockfd();
}

inline SocketAddress Socket::address() const {
    return _pImpl->address();
}

inline SocketAddress Socket::peerAddress() const {
    return _pImpl->peerAddress();
}

inline bool Socket::poll(const Timespan& timeout, int mode) const {
    return _pImpl->poll(timeout, mode);
}

inline void Socket::setSendTimeout(const Timespan& timeout) {
    _pImpl->setSendTimeout(timeout);
}

inline Timespan Socket::getSendTimeout() {
    return _pImpl->getSendTimeout();
}

inline void Socket::setReceiveTimeout(const Timespan& timeout) {
    _pImpl->setReceiveTimeout(timeout);
}

inline Timespan Socket::getReceiveTimeout() {
    return _pImpl->getReceiveTimeout();
}

} // namespace lyx

#endif // LIBLYX_NETWORK_LYXSOCKET_H_
