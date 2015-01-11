#include "lyxHTTPClientSession.h"
#include "lyxHTTPSession.h"
#include "lyxNetException.h"
#include "lyxNumberFormatter.h"

namespace lyx {

HTTPClientSession::HTTPClientSession():
    _port(HTTPSession::HTTP_PORT),
    _proxyPort(HTTPSession::HTTP_PORT),
    _keepAliveTimeout(DEFAULT_KEEP_ALIVE_TIMEOUT, 0),
    _reconnect(false),
    _mustReconnect(false),
    _expectResponseBody(false)
{
}

HTTPClientSession::HTTPClientSession(const StreamSocket& socket):
    HTTPSession(socket),
    _port(HTTPSession::HTTP_PORT),
    _proxyPort(HTTPSession::HTTP_PORT),
    _keepAliveTimeout(DEFAULT_KEEP_ALIVE_TIMEOUT, 0),
    _reconnect(false),
    _mustReconnect(false),
    _expectResponseBody(false)
{
}

HTTPClientSession::HTTPClientSession(const SocketAddress& address):
    _host(address.host().toString()),
    _port(address.port()),
    _proxyPort(HTTPSession::HTTP_PORT),
    _keepAliveTimeout(DEFAULT_KEEP_ALIVE_TIMEOUT, 0),
    _reconnect(false),
    _mustReconnect(false),
    _expectResponseBody(false)
{
}

HTTPClientSession::HTTPClientSession(const std::string& host, uint16_t port):
    _host(host),
    _port(port),
    _proxyPort(HTTPSession::HTTP_PORT),
    _keepAliveTimeout(DEFAULT_KEEP_ALIVE_TIMEOUT, 0),
    _reconnect(false),
    _mustReconnect(false),
    _expectResponseBody(false)
{
}

HTTPClientSession::~HTTPClientSession() {
}

void HTTPClientSession::setHost(const std::string& host) {
    if (!connected())
        _host = host;
    else
        throw IllegalStateException("Cannot set the host for an already connected session");
}

void HTTPClientSession::setPort(uint16_t port) {
    if (!connected())
        _port = port;
    else
        throw IllegalStateException("Cannot set the port number for an already connected session");
}

void HTTPClientSession::setProxy(const std::string& host, uint16_t port) {
    if (!connected()) {
        _proxyHost = host;
        _proxyPort = port;
    }
    else
        throw IllegalStateException("Cannot set the proxy host and port for an already connected session");
}

void HTTPClientSession::setProxyHost(const std::string& host) {
    if (!connected())
        _proxyHost = host;
    else
        throw IllegalStateException("Cannot set the proxy host for an already connected session");
}

void HTTPClientSession::setProxyPort(uint16_t port) {
    if (!connected())
        _proxyPort = port;
    else
        throw IllegalStateException("Cannot set the proxy port number for an already connected session");
}

void HTTPClientSession::setProxyCredentials(const std::string& username, const std::string& password) {
    _proxyUsername = username;
    _proxyPassword = password;
}

void HTTPClientSession::setProxyUsername(const std::string& username) {
    _proxyUsername = username;
}

void HTTPClientSession::setProxyPassword(const std::string& password) {
    _proxyPassword = password;
}

void HTTPClientSession::setKeepAliveTimeout(const Timespan& timeout) {
    _keepAliveTimeout = timeout;
}

// std::ostream& sendRequest(HTTPRequest& request);
// std::istream& receiveResponse(HTTPResponse& response);

void HTTPClientSession::reset() {
    close();
}

bool HTTPClientSession::secure() const {
    return false;
}

int HTTPClientSession::write(const char* buffer, std::streamsize length) {
    try {
        int rc = HTTPSession::write(buffer, length);
        _reconnect = false;
        return rc;
    }
    catch (NetException&) {
        if (_reconnect) {
            close();
            reconnect();
            int rc = HTTPClientSession::write(buffer, length);
            _reconnect = false;
            return rc;
        }
        else
            throw;
    }
}

void HTTPClientSession::reconnect() {
    if (_proxyHost.empty()) {
        SocketAddress addr(_host, _port);
        connect(addr);
    }
    else {
        SocketAddress addr(_proxyHost, _proxyPort);
        connect(addr);
    }
}

std::string HTTPClientSession::proxyRequestPrefix() const {
    std::string result("http://");
    result.append(_host);
    result.append(":");
    NumberFormatter::append(result, _port);
    return result;
}

bool HTTPClientSession::mustReconnect() const {
    if (!_mustReconnect) {
        Timestamp now;
        return _keepAliveTimeout <= now - _lastRequest;
    }
    else
        return true;
}

// void proxyAuthenticate(HTTPRequest& request);
// void proxyAuthenticateImpl(HTTPRequest& request);

StreamSocket HTTPClientSession::proxyConnect() {
    HTTPClientSession proxySession(getProxyHost(), getProxyPort());
    proxySession.setTimeout(getTimeout());

    std::string targetAddress(_host);
    targetAddress.append(":");
    NumberFormatter::append(targetAddress, _port);

    throw NotImplementedException("HTTPClientSession::proxyConnect() not implement");
}

void HTTPClientSession::proxyTunnel() {
    StreamSocket ss = proxyConnect();
    attachSocket(ss);
}

} // namespace lyx
