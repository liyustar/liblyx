#ifndef LIBLYX_NETWORK_LYXSOCKETIMPL_H_
#define LIBLYX_NETWORK_LYXSOCKETIMPL_H_

#include "lyxSocketAddress.h"
#include "lyxTimespan.h"
#include <string>

namespace lyx {

class SocketImpl {
    public:
        enum SelectMode {
            SELECT_READ     = 1,
            SELECT_WRITE    = 2,
            SELECT_ERROR    = 4
        };

        virtual SocketImpl* acceptConnection(SocketAddress& clientAddr);
        virtual void connect(const SocketAddress& address);
        virtual void connect(const SocketAddress& address, const Timespan& timeout);
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

        virtual bool poll(const Timespan& timeout, int mode);

        virtual void setBlocking(bool flag);

        int socketError();
        int sockfd() const;

        // virtual int available();

    // protected:
        SocketImpl();
        SocketImpl(int sockfd);
        ~SocketImpl();

        virtual void init(int af);
        virtual void initSocket(int af, int type, int proto = 0);

        void getOption(int level, int option, int& value);

        static int lastError();
        static void error();
        static void error(int code);
        static void error(const std::string& arg);
        static void error(int code, const std::string& arg);

    private:
        SocketImpl(const SocketImpl&);
        SocketImpl& operator = (const SocketImpl&);

        int  _sockfd;
        bool _blocking;

        friend class Socket;
};

inline int SocketImpl::sockfd() const {
    return _sockfd;
}

inline int SocketImpl::lastError() {
    return errno;
}

} // namespace lyx

#endif // LIBLYX_NETWORK_LYXSOCKETIMPL_H_
