#include "lyxHTTPMessage.h"
#include "lyxMediaType.h"
#include "lyxNumberFormatter.h"

namespace lyx {

const std::string HTTPMessage::HTTP_1_0 = "HTTP/1.0";
const std::string HTTPMessage::HTTP_1_1 = "HTTP/1.1";

const std::string HTTPMessage::IDENTITY_TRANSFER_ENCODING = "identity";
const std::string HTTPMessage::CHUNKED_TRANSFER_ENCODING  = "chunked";

const int         HTTPMessage::UNKNOWN_CONTENT_LENGTH     = -1;
const std::string HTTPMessage::UNKNOWN_CONTENT_TYPE;

const std::string HTTPMessage::CONTENT_LENGTH    = "Content-Length";
const std::string HTTPMessage::CONTENT_TYPE      = "Content-Type";
const std::string HTTPMessage::TRANSFER_ENCODING = "Transfer-Encoding";
const std::string HTTPMessage::CONNECTION        = "Connection";

const std::string HTTPMessage::CONNECT_KEEP_ALIVE = "Keep-Alive";
const std::string HTTPMessage::CONNECT_CLOSE      = "Close";

const std::string HTTPMessage::EMPTY;

HTTPMessage::HTTPMessage():
    _version(HTTP_1_0)
{
}

HTTPMessage::HTTPMessage(const std::string& version):
    _version(version)
{
}

HTTPMessage::~HTTPMessage() {
}

void HTTPMessage::setVersion(const std::string& version) {
    _version = version;
}

void HTTPMessage::setContentLength(std::streamsize length) {
    if (length != UNKNOWN_CONTENT_LENGTH)
        set(CONTENT_LENGTH, NumberFormatter::format(length));
    else
        erase(CONTENT_LENGTH);
}

std::streamsize HTTPMessage::getContentLength() const {
    const std::string& contentLength = get(CONTENT_LENGTH, EMPTY);
    if (!contentLength.empty())
        return static_cast<std::streamsize>(std::atoi(contentLength.c_str()));
    else
        return UNKNOWN_CONTENT_LENGTH;
}

void HTTPMessage::setTransferEncoding(const std::string& transferEncoding) {
    if (transferEncoding.compare(IDENTITY_TRANSFER_ENCODING) == 0)
        erase(TRANSFER_ENCODING);
    else
        set(TRANSFER_ENCODING, transferEncoding);
}

const std::string& HTTPMessage::getTransferEncoding() const {
    return get(TRANSFER_ENCODING, IDENTITY_TRANSFER_ENCODING);
}

void HTTPMessage::setChunkedTransferEncoding(bool flag) {
    if (flag)
        setTransferEncoding(CHUNKED_TRANSFER_ENCODING);
    else
        setTransferEncoding(IDENTITY_TRANSFER_ENCODING);
}

bool HTTPMessage::getChunkedTransferEncoding() const {
    return getTransferEncoding().compare(CHUNKED_TRANSFER_ENCODING) == 0;
}

void HTTPMessage::setContentType(const std::string& mediaType) {
    if (mediaType.empty())
        erase(CONTENT_TYPE);
    else
        set(CONTENT_TYPE, mediaType);
}

void HTTPMessage::setContentType(const MediaType& mediaType) {
    setContentType(mediaType.toString());
}

const std::string& HTTPMessage::getContentType() const {
    return get(CONTENT_TYPE, UNKNOWN_CONTENT_TYPE);
}

void HTTPMessage::setKeepAlive(bool keepAlive) {
    if (keepAlive)
        set(CONNECTION, CONNECT_KEEP_ALIVE);
    else
        set(CONNECTION, CONNECT_CLOSE);
}

bool HTTPMessage::getKeepAlive() const {
    const std::string& connection = get(CONNECTION, EMPTY);
    if (!connection.empty())
        return connection.compare(CONNECT_CLOSE) != 0;
    else
        return getVersion() == HTTP_1_1;
}

} // namespace lyx
