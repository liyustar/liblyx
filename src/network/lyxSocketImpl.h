#ifndef LIBLYX_NETWORK_LYXSOCKETIMPL_H_
#define LIBLYX_NETWORK_LYXSOCKETIMPL_H_

#include "lyxSocketAddress.h"

namespace lyx {

class SocketImpl {
    public:
        virtual void connect(const SocketAddress& address);

        virtual void close();
        virtual void shutdownReceive();
        virtual void shutdownSend();
        virtual void shutdown();

        virtual int sendBytes(const void* buffer, int length, int flags = 0);
        virtual int receiveBytes(void* buffer, int length, int flags = 0);

        virtual int available();

    // protected:
        SocketImpl();
        SocketImpl(int sockfd);
        ~SocketImpl();

    private:
        SocketImpl(const SocketImpl&);
        SocketImpl& operator = (const SocketImpl&);

        int _sockfd;
        bool _blocking;
};

} // namespace lyx

#endif // LIBLYX_NETWORK_LYXSOCKETIMPL_H_
