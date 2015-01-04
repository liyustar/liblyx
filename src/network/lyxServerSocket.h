#ifndef LIBLYX_NETWORK_LYXSERVERSOCKET_H_
#define LIBLYX_NETWORK_LYXSERVERSOCKET_H_

#include "lyxSocket.h"
#include "lyxStreamSocket.h"

namespace lyx {

class ServerSocket: public Socket {
    public:
        ServerSocket();
        ServerSocket(const Socket& socket);
        ServerSocket(const SocketAddress& address, int backlog = 64);
        ServerSocket(uint16_t port, int backlog = 64);
        virtual ~ServerSocket();

        ServerSocket& operator = (const Socket& socket);

        virtual void bind(const SocketAddress& address, bool reuseAddress = false);
        virtual void bind(uint16_t port, bool reuseAddress = false);
        virtual void listen(int backlog = 64);

        virtual StreamSocket acceptConnection(SocketAddress& clientAddr);
        virtual StreamSocket acceptConnection();

};

} // namespace lyx

#endif // LIBLYX_NETWORK_LYXSERVERSOCKET_H_
