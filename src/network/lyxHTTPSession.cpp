#include "lyxHTTPSession.h"
#include "lyxNetException.h"

namespace lyx {

HTTPSession::HTTPSession():
    _pBuffer(0),
    _pCurrent(0),
    _pEnd(0),
    _timeout(HTTP_DEFAULT_TIMEOUT),
    _pException(0)
{
}

HTTPSession::HTTPSession(const StreamSocket& socket):
    _socket(socket),
    _pBuffer(0),
    _pCurrent(0),
    _pEnd(0),
    _timeout(HTTP_DEFAULT_TIMEOUT),
    _pException(0)
{
}

void HTTPSession::setTimeout(const Timespan& timeout) {
    _timeout = timeout;
}

Timespan HTTPSession::getTimeout() const {
    return _timeout;
}


} // namespace lyx
