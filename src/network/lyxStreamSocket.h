#ifndef LIBLYX_NETWORK_LYXSTREAMSOCKET_H_
#define LIBLYX_NETWORK_LYXSTREAMSOCKET_H_

#include "lyxSocket.h"
#include <memory>
#include "lyxStreamSocketImpl.h"

namespace lyx {

class StreamSocket : public Socket {
    public:
        typedef std::shared_ptr<StreamSocketImpl> Ptr;

        StreamSocket();
        explicit StreamSocket(const SocketAddress& address);
        explicit StreamSocket(IPAddress::Family family);
        StreamSocket(const Socket& socket);

        virtual ~StreamSocket();

        StreamSocket& operator = (const Socket& socket);

        void connect(const SocketAddress& address);

        void shutdownReceive();
        void shutdownSend();
        void shutdown();

        int sendBytes(const void* buffer, int length, int flags = 0);
        int receiveBytes(void* buffer, int length, int flags = 0);

        StreamSocket(SocketImpl* pImpl);

    private:
        enum {
            BUFFER_SIZE = 1024
        };
};

} // namespace lyx

#endif // LIBLYX_NETWORK_LYXSTREAMSOCKET_H_
