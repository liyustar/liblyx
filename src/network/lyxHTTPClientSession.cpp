#include "lyxHTTPClientSession.h"
#include "lyxHTTPSession.h"
#include "lyxHTTPRequest.h"
#include "lyxHTTPResponse.h"
#include "lyxHTTPFixedLengthStream.h"
#include "lyxCountingStream.h"
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

std::ostream& HTTPClientSession::sendRequest(HTTPRequest& request) {
    clearException();
    _pResponseStream = 0;

    bool keepAlive = getKeepAlive();
    // 检查是否需要重新建立链接
    if ((connected() && !keepAlive) || mustReconnect()) {
        close();
        _mustReconnect = false;
    }

    try {
        if (!connected())
            reconnect();
        if (!keepAlive)
            request.setKeepAlive(false);
        if (!request.has(HTTPRequest::HOST))
            request.setHost(_host, _port);

        _reconnect = keepAlive;
        _expectResponseBody = request.getMethod() != HTTPRequest::HTTP_HEAD;

        if (request.getChunkedTransferEncoding()) {
            throw NotImplementedException("HTTPClientSession::sendRequest can't process Chunked");
        }
        else if (request.hasContentLength()) {
            CountingOutputStream cs;
            request.write(cs);
            _pRequestStream.reset(new HTTPFixedLengthOutputStream(*this, request.getContentLength() + cs.chars()));
            request.write(*_pRequestStream);
        }
        else if ((request.getMethod() != HTTPRequest::HTTP_PUT
                    && request.getMethod() != HTTPRequest::HTTP_POST)
                || request.has(HTTPRequest::UPGRADE)) {
            throw NotImplementedException("HTTPClientSession::sendRequest can't process hasUpGrade");
        }
        else {
            throw NotImplementedException("HTTPClientSession::sendRequest can't process Normal");
        }
        _lastRequest.update();
        return *_pRequestStream;
    }
    catch (Exception&) {
        close();
        throw;
    }
}

std::istream& HTTPClientSession::receiveResponse(HTTPResponse& response) {
    _pRequestStream = 0;
    if (networkException()) networkException()->rethrow();

    do {
        response.clear();
    } while (false /* response.getStatus() == HTTPResponse::HTTP_CONTINUE */);

    _mustReconnect = getKeepAlive() && !response.getKeepAlive();

    if (!_expectResponseBody || response.getStatus() < 200
            || response.getStatus() == HTTPResponse::HTTP_NO_CONTENT
            || response.getStatus() == HTTPResponse::HTTP_NOT_MODIFIED)
        _pResponseStream.reset(new HTTPFixedLengthInputStream(*this, 0));
    else if (response.getChunkedTransferEncoding())
        throw NotImplementedException("HTTPClientSession::receiveResponse ChunkedInputStream");
    else if (response.hasContentLength())
        _pResponseStream.reset(new HTTPFixedLengthInputStream(*this, response.getContentLength()));
    else
        throw NotImplementedException("HTTPClientSession::receiveResponse HTTPIOStream");

    return *_pResponseStream;
}

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
