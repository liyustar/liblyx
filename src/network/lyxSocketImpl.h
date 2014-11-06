#ifndef LIBLYX_NETWORK_LYXSOCKETIMPL_H_
#define LIBLYX_NETWORK_LYXSOCKETIMPL_H_

#include "lyxSocketAddress.h"

namespace lyx {

class SocketImpl {
    public:
        virtual SocketImpl* acceptConnection(SocketAddress& clientAddr);
        virtual void connect(const SocketAddress& address);
        virtual void bind(const SocketAddress& address, bool reuseAddress = false);
        virtual void listen(int backlog = 64);

        virtual void close();
        virtual void shutdownReceive();
        virtual void shutdownSend();
        virtual void shutdown();

        virtual int sendBytes(const void* buffer, int length, int flags = 0);
        virtual int receiveBytes(void* buffer, int length, int flags = 0);

        virtual SocketAddress address() const;
        virtual SocketAddress peerAddress() const;

        int sockfd() const;

        // virtual int available();

    // protected:
        SocketImpl();
        SocketImpl(int sockfd);
        ~SocketImpl();

        virtual void init(int af);
        virtual void initSocket(int af, int type, int proto = 0);

    private:
        SocketImpl(const SocketImpl&);
        SocketImpl& operator = (const SocketImpl&);

        int _sockfd;
        bool _blocking;

        friend class Socket;
};

inline int SocketImpl::sockfd() const {
    return _sockfd;
}

} // namespace lyx

#endif // LIBLYX_NETWORK_LYXSOCKETIMPL_H_
