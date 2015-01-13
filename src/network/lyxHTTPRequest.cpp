#include "lyxHTTPRequest.h"
#include "lyxAscii.h"
#include "lyxNumberFormatter.h"
#include "lyxNetException.h"

namespace lyx {

const std::string HTTP_GET            = "GET";
const std::string HTTP_HEAD           = "HEAD";
const std::string HTTP_PUT            = "PUT";
const std::string HTTP_POST           = "POST";
const std::string HTTP_OPTIONS        = "OPTIONS";
const std::string HTTP_DELETE         = "DELETE";
const std::string HTTP_TRACE          = "TRACE";
const std::string HTTP_CONNECT        = "CONNECT";
const std::string HOST                = "Host";
const std::string COOKIE              = "Cookie";
const std::string AUTHORIZATION       = "Authorization";
const std::string PROXY_AUTHORIZATION = "Proxy-Authorization";
const std::string UPGRADE             = "Upgrade";

HTTPRequest::HTTPRequest():
    _method(HTTP_GET),
    _uri("/")
{
}

HTTPRequest::HTTPRequest(const std::string& version):
    HTTPMessage(version),
    _method(HTTP_GET),
    _uri("/")
{
}

HTTPRequest::HTTPRequest(const std::string& method, const std::string& uri):
    _method(method),
    _uri(uri)
{
}

HTTPRequest::HTTPRequest(const std::string& method, const std::string& uri, const std::string& version):
    HTTPMessage(version),
    _method(method),
    _uri(uri)
{
}

HTTPRequest::~HTTPRequest() {
}

void HTTPRequest::setMethod(const std::string& method) {
    _method = method;
}

void HTTPRequest::setURI(const std::string& uri) {
    _uri = uri;
}

void HTTPRequest::setHost(const std::string& host) {
    set(HOST, host);
}

void HTTPRequest::setHost(const std::string& host, uint16_t port) {
    std::string value;

    value.append(host);

    if (port != 80 && port != 443) {
        value.append(":");
        NumberFormatter::append(value, port);
    }
    setHost(value);
}

const std::string& HTTPRequest::getHost() const {
    return get(HOST);
}

void HTTPRequest::setCookies(const NameValueCollection& cookies) {
    std::string cookie;
    cookie.reserve(64);
    for (NameValueCollection::ConstIterator it = cookies.begin(); it != cookies.end(); ++it) {
        if (it != cookies.begin())
            cookie.append("; ");
        cookie.append(it->first);
        cookie.append("=");
        cookie.append(it->second);
    }
    add(COOKIE, cookie);
}

void HTTPRequest::getCookies(NameValueCollection& cookies) const {
    NameValueCollection::ConstIterator it = find(COOKIE);
    while (it != end() && it->first == COOKIE) {
        splitParameters(it->second.begin(), it->second.end(), cookies);
        ++it;
    }
}

void HTTPRequest::write(std::ostream& ostr) const {
    ostr << _method << " " << _uri << " " << getVersion() << "\r\n";
    HTTPMessage::write(ostr);
    ostr << "\r\n";
}

void HTTPRequest::read(std::istream& istr) {
    static const int eof = std::char_traits<char>::eof();

    std::string method;
    std::string uri;
    std::string version;
    method.reserve(16);
    uri.reserve(64);
    version.reserve(64);

    int ch = istr.get();
    if (istr.bad()) throw NetException("Network failure while reading HTTP request header");
    if (istr.bad()) throw NetException("Error reading HTTP request header");
    if (ch == eof) throw NoMessageException();

    while (Ascii::isSpace(ch)) ch = istr.get();
    if (ch == eof) throw MessageException("No HTTP request header");

    while (!Ascii::isSpace(ch) && ch != eof && method.length() < MAX_METHOD_LENGTH) {
        method += (char) ch;
        ch = istr.get();
    }
    if (!Ascii::isSpace(ch)) throw MessageException("HTTP request method invalid or too long");

    while (Ascii::isSpace(ch)) ch = istr.get();
    while (!Ascii::isSpace(ch) && ch != eof && uri.length() < MAX_URI_LENGTH) {
        uri += (char) ch;
        ch = istr.get();
    }
    if (!Ascii::isSpace(ch)) throw MessageException("HTTP request URI invalid or too long");

    while (Ascii::isSpace(ch)) ch = istr.get();
    while (!Ascii::isSpace(ch) && ch != eof && version.length() < MAX_VERSION_LENGTH) {
        version += (char) ch;
        ch = istr.get();
    }
    if (!Ascii::isSpace(ch)) throw MessageException("Invalid HTTP version string");

    while (ch != '\n' && ch != eof) {
        ch = istr.get();
    }
    HTTPMessage::read(istr);

    ch = istr.get();
    while (ch != '\n' && ch != eof) {
        ch = istr.get();
    }
    setMethod(method);
    setURI(uri);
    setVersion(version);
}

} // namespace lyx
