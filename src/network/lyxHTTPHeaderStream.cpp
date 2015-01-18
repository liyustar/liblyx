#include "lyxHTTPHeaderStream.h"
#include "lyxHTTPSession.h"
#include "lyxBugcheck.h"

namespace lyx {

HTTPHeaderStreamBuf::HTTPHeaderStreamBuf(HTTPSession& session, openmode mode):
    HTTPBasicStreamBuf(HTTPBufferAllocator::BUFFER_SIZE, mode),
    _session(session),
    _end(false)
{
}

HTTPHeaderStreamBuf::~HTTPHeaderStreamBuf() {
}

int HTTPHeaderStreamBuf::readFromDevice(char* buffer, std::streamsize length) {
    // read line-by-line; an empty line denotes the end of the headers.
    static const int eof = std::char_traits<char>::eof();

    if (_end) return 0;

    int n = 0;
    int ch = _session.get();
    while (ch != eof && ch != '\n' && n < length - 1) {
        *buffer++ = (char) ch; ++n;
        ch = _session.get();
    }
    if (ch != eof) {
        *buffer++ = (char) ch; ++n;
        if (n == 2) _end = true;
    }
    return n;
}

int HTTPHeaderStreamBuf::writeToDevice(const char* buffer, std::streamsize length) {
    return _session.write(buffer, length);
}

HTTPHeaderIOS::HTTPHeaderIOS(HTTPSession& session, HTTPHeaderStreamBuf::openmode mode):
    _buf(session, mode)
{
}

HTTPHeaderIOS::~HTTPHeaderIOS() {
    try {
        _buf.sync();
    }
    catch (...) {
    }
}

HTTPHeaderStreamBuf* HTTPHeaderIOS::rdbuf() {
    return &_buf;
}

MemoryPool HTTPHeaderInputStream::_pool(sizeof(HTTPHeaderInputStream));

HTTPHeaderInputStream::HTTPHeaderInputStream(HTTPSession& session):
    HTTPHeaderIOS(session, std::ios::in),
    std::istream(&_buf)
{
}

HTTPHeaderInputStream::~HTTPHeaderInputStream() {
}

void* HTTPHeaderInputStream::operator new(std::size_t size) {
    return _pool.get();
}

void HTTPHeaderInputStream::operator delete(void* ptr) {
    try {
        _pool.release(ptr);
    }
    catch (...) {
        lyx_unexpected();
    }
}

MemoryPool HTTPHeaderOutputStream::_pool(sizeof(HTTPHeaderOutputStream));

HTTPHeaderOutputStream::HTTPHeaderOutputStream(HTTPSession& session):
    HTTPHeaderIOS(session, std::ios::out),
    std::ostream(&_buf)
{
}

HTTPHeaderOutputStream::~HTTPHeaderOutputStream() {
}

void* HTTPHeaderOutputStream::operator new(std::size_t size) {
    return _pool.get();
}

void HTTPHeaderOutputStream::operator delete(void* ptr) {
    try {
        _pool.release(ptr);
    }
    catch (...) {
        lyx_unexpected();
    }
}

} // namespace lyx
