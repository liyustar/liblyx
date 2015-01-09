#ifndef LIBLYX_NETWORK_LYXHTTPSESSION_H_
#define LIBLYX_NETWORK_LYXHTTPSESSION_H_

#include "lyxStreamSocket.h"
#include "lyxTimespan.h"
#include "lyxException.h"
#include <ios>

namespace lyx {

class HTTPSession {
    public:
        void setTimeout(const Timespan& timeout);
        Timespan getTimeout() const;

        bool connected() const;
        virtual void abort();
        const Exception* networkException() const;

        enum {
            HTTP_PORT = 80
        };

        StreamSocket detachSocket();
        StreamSocket& socket();

    protected:
        HTTPSession();
        HTTPSession(const StreamSocket& socket);
        HTTPSession(const StreamSocket& socket, bool keepAlive);
        virtual ~HTTPSession();

        int get();
        int peek();
        virtual int read(char* buffer, std::streamsize length);
        virtual int write(const char* buffer, std::streamsize length);
        int receive(char* buffer, int length);
        int buffered() const;
        void refill();

        virtual void connect(const SocketAddress& address);
        void attachSocket(const StreamSocket& socket);
        void close();

        void setException(const Exception& exc);
        void clearException();

    private:
        enum {
            HTTP_DEFAULT_TIMEOUT = 60000000
        };

        HTTPSession(const HTTPSession&);
        HTTPSession& operator = (const HTTPSession&);

        StreamSocket    _socket;
        char*           _pBuffer;
        char*           _pCurrent;
        char*           _pEnd;
        Timespan        _timeout;
        Exception*      _pException;
};

} // namespace lyx

#endif // LIBLYX_NETWORK_LYXHTTPSESSION_H_
