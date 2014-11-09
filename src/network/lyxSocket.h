#ifndef LIBLYX_NETWORK_LYXSOCKET_H_
#define LIBLYX_NETWORK_LYXSOCKET_H_

#include "lyxSocketImpl.h"
#include <vector>
#include <memory>

namespace lyx {

class Socket {
    public:
        typedef std::vector<Socket> SocketList;
        typedef std::shared_ptr<SocketImpl> Ptr;

        Socket();
        Socket(const Socket& socket);

        Socket& operator = (const Socket& socket);
        virtual ~Socket();

        void close();

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

} // namespace lyx

#endif // LIBLYX_NETWORK_LYXSOCKET_H_
