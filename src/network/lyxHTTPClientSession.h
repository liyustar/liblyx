#ifndef LIBLYX_NETWORK_LYXHTTPCLIENTSESSION_H_
#define LIBLYX_NETWORK_LYXHTTPCLIENTSESSION_H_

#include "lyxHTTPSession.h"
#include "lyxTimespan.h"
#include "lyxTimestamp.h"

namespace lyx {

class HTTPRequest;
class HTTPResponse;

class HTTPClientSession: public HTTPSession {
    public:
        HTTPClientSession();
        explicit HTTPClientSession(const StreamSocket& socket);
        explicit HTTPClientSession(const SocketAddress& address);
        HTTPClientSession(const std::string& host, uint16_t port = HTTPSession::HTTP_PORT);
        virtual ~HTTPClientSession();

        void setHost(const std::string& host);
        void setPort(uint16_t port);
        const std::string& getHost() const;
        uint16_t getPort() const;

        void setProxy(const std::string& host, uint16_t port = HTTPSession::HTTP_PORT);
        void setProxyHost(const std::string& host);
        void setProxyPort(uint16_t port);
        const std::string& getProxyHost() const;
        uint16_t getProxyPort() const;

        void setProxyCredentials(const std::string& username, const std::string& password);
        void setProxyUsername(const std::string& username);
        void setProxyPassword(const std::string& password);
        const std::string& getProxyUsername() const;
        const std::string& getProxyPassword() const;

        void setKeepAliveTimeout(const Timespan& timeout);
        const Timespan& getKeepAliveTimeout() const;

        virtual std::ostream& sendRequest(HTTPRequest& request);
        virtual std::istream& receiveResponse(HTTPResponse& response);

        void reset();
        virtual bool secure() const;

    protected:
        enum {
            DEFAULT_KEEP_ALIVE_TIMEOUT = 0
        };

        void reconnect();
        int write(const char* buffer, std::streamsize length);
        virtual std::string proxyRequestPrefix() const;
        virtual bool mustReconnect() const;
        virtual void proxyAuthenticate(HTTPRequest& request);
        void proxyAuthenticateImpl(HTTPRequest& request);

        StreamSocket proxyConnect();
        void proxyTunnel();

    private:
        std::string     _host;
        uint16_t        _port;
        std::string     _proxyHost;
        uint16_t        _proxyPort;
        std::string     _proxyUsername;
        std::string     _proxyPassword;
        Timespan        _keepAliveTimeout;
        Timestamp       _lastRequest;
        bool            _reconnect;
        bool            _mustReconnect;
        bool            _expectResponseBody;
        std::shared_ptr<std::ostream> _pRequestStream;
        std::shared_ptr<std::istream> _pResponseStream;

        HTTPClientSession(const HTTPClientSession&);
        HTTPClientSession& operator = (const HTTPClientSession&);
};

inline const std::string& HTTPClientSession::getHost() const {
    return _host;
}

inline uint16_t HTTPClientSession::getPort() const {
    return _port;
}

inline const std::string& HTTPClientSession::getProxyHost() const {
    return _proxyHost;
}

inline uint16_t HTTPClientSession::getProxyPort() const {
    return _proxyPort;
}

inline const std::string& HTTPClientSession::getProxyUsername() const {
    return _proxyUsername;
}

inline const std::string& HTTPClientSession::getProxyPassword() const {
    return _proxyPassword;
}

inline const Timespan& HTTPClientSession::getKeepAliveTimeout() const {
    return _keepAliveTimeout;
}

} // namespace lyx

#endif // LIBLYX_NETWORK_LYXHTTPCLIENTSESSION_H_
