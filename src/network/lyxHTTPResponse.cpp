#include "lyxHTTPResponse.h"
#include "lyxAscii.h"
#include "lyxString.h"
#include "lyxNetException.h"

namespace lyx {

const std::string HTTPResponse::HTTP_REASON_OK        = "OK";
const std::string HTTPResponse::HTTP_REASON_SEE_OTHER = "See Other";
const std::string HTTPResponse::HTTP_REASON_NOT_FOUND = "Not Found";
const std::string HTTPResponse::HTTP_REASON_UNKNOWN   = "???";
const std::string HTTPResponse::DATE                  = "Date";
const std::string HTTPResponse::SET_COOKIE            = "Set-Cookie";

HTTPResponse::HTTPResponse():
    _status(HTTP_OK),
    _reason(getReasonForStatus(HTTP_OK))
{
}

HTTPResponse::HTTPResponse(HTTPStatus status, const std::string& reason):
    _status(status),
    _reason(reason)
{
}

HTTPResponse::HTTPResponse(const std::string& version, HTTPStatus status, const std::string& reason):
    HTTPMessage(version),
    _status(status),
    _reason(reason)
{
}

HTTPResponse::HTTPResponse(HTTPStatus status):
    _status(status),
    _reason(getReasonForStatus(status))
{
}

HTTPResponse::HTTPResponse(const std::string& version, HTTPStatus status):
    HTTPMessage(version),
    _status(status),
    _reason(getReasonForStatus(status))
{
}

HTTPResponse::~HTTPResponse() {
}

void HTTPResponse::setStatus(HTTPStatus status) {
    _status = status;
}

void HTTPResponse::setStatus(const std::string& status) {
    throw NotImplementedException("HTTPResponse::setStatus");
}

void HTTPResponse::setReason(const std::string& reason) {
    _reason = reason;
}

void HTTPResponse::setStatusAndReason(HTTPStatus status, const std::string& reason) {
    _status = status;
    _reason = reason;
}

void HTTPResponse::setStatusAndReason(HTTPStatus status) {
    setStatusAndReason(status, getReasonForStatus(status));
}

void HTTPResponse::setDate(const Timestamp& dateTime) {
    throw NotImplementedException("HTTPResponse::setDate");
}

Timestamp HTTPResponse::getDate() const {
    const std::string& dateTime = get(DATE);
    throw NotImplementedException("HTTPResponse::getDate");
}

void HTTPResponse::addCookie(const HTTPCookie& cookie) {
    add(SET_COOKIE, cookie.toString());
}

void HTTPResponse::getCookies(std::vector<HTTPCookie>& cookies) const {
    cookies.clear();
    NameValueCollection::ConstIterator it = find(SET_COOKIE);
    while (it != end() && icompare(it->first, SET_COOKIE) == 0) {
        NameValueCollection nvc;
        splitParameters(it->second.begin(), it->second.end(), nvc);
        cookies.push_back(HTTPCookie(nvc));
        ++it;
    }
}

void HTTPResponse::write(std::ostream& ostr) const {
    ostr << getVersion() << " " << static_cast<int>(_status) << " " << _reason << "\r\n";
    HTTPMessage::write(ostr);
    ostr << "\r\n";
}

void HTTPResponse::read(std::istream& istr) {
    static const int eof = std::char_traits<char>::eof();

    std::string version;
    std::string status;
    std::string reason;

    int ch = istr.get();
    if (ch == eof) throw NoMessageException();
    while (ch != eof && !Ascii::isSpace(ch) && version.length() < MAX_VERSION_LENGTH) {
        version += (char) ch;
        ch = istr.get();
    }
    if (!Ascii::isSpace(ch)) throw MessageException("Invalid HTTP version string");

    while (Ascii::isSpace(ch)) ch = istr.get();
    while (ch != eof && !Ascii::isSpace(ch) && status.length() < MAX_STATUS_LENGTH) {
        status += (char) ch;
        ch = istr.get();
    }
    if (!Ascii::isSpace(ch)) throw MessageException("Invalid HTTP status code");
    while (Ascii::isSpace(ch) && ch != '\r' && ch != '\n' && ch != eof) ch = istr.get();
    while (ch != eof && ch != '\r' && ch != '\n' && reason.length() < MAX_REASON_LENGTH) {
        reason += (char) ch;
        ch = istr.get();
    }
    if (!Ascii::isSpace(ch)) throw MessageException("HTTP reason string too long");
    if (ch == '\r') ch = istr.get();

    HTTPMessage::read(istr);
    ch = istr.get();
    while (ch != '\n' && ch != eof) { ch = istr.get(); }
    setVersion(version);
    setStatus(status);
    setReason(reason);
}

const std::string& HTTPResponse::getReasonForStatus(HTTPStatus status) {
    switch (status) {
        case HTTP_OK:
            return HTTP_REASON_OK;
        case HTTP_SEE_OTHER:
            return HTTP_REASON_SEE_OTHER;
        case HTTP_NOT_FOUND:
            return HTTP_REASON_NOT_FOUND;
        default:
            return HTTP_REASON_UNKNOWN;
    }
}

} // namespace lyx
