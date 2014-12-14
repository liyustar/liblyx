#include "lyxDigestStream.h"

namespace lyx {

const int DigestBuf::BUFFER_SIZE = 256;

DigestBuf::DigestBuf(DigestEngine& eng):
    BufferedStreamBuf(BUFFER_SIZE, std::ios::out),
    _eng(eng),
    _pIstr(0),
    _pOstr(0)
{
}

DigestBuf::DigestBuf(DigestEngine& eng, std::istream& istr):
    BufferedStreamBuf(BUFFER_SIZE, std::ios::in),
    _eng(eng),
    _pIstr(&istr),
    _pOstr(0)
{
}

DigestBuf::DigestBuf(DigestEngine& eng, std::ostream& ostr):
    BufferedStreamBuf(BUFFER_SIZE, std::ios::out),
    _eng(eng),
    _pIstr(0),
    _pOstr(&ostr)
{
}

DigestBuf::~DigestBuf() {
}

int DigestBuf::readFromDevice(char* buffer, std::streamsize length) {
    if (_pIstr && _pIstr->good()) {
        _pIstr->read(buffer, length);
        std::streamsize n = _pIstr->gcount();
        if (n > 0) _eng.update(buffer, static_cast<unsigned>(n));
        return static_cast<int>(n);
    }
    return -1;
}

void DigestBuf::close() {
    sync();
    if (_pOstr) _pOstr->flush();
}

DigestIOS::DigestIOS(DigestEngine& eng): _buf(eng) {
    init(&_buf);
}

DigestIOS::~DigestIOS() {
}

DigestBuf* DigestIOS::rdbuf() {
    return &_buf;
}

DigestInputStream::DigestInputStream(DigestEngine& eng, std::istream& istr):
    DigestIOS(eng, istr),
    std::istream(&_buf)
{
}

DigestInputStream::~DigestInputStream() {
}

DigestOutputStream::DigestOutputStream(DigestEngine& eng):
    DigestIOS(eng),
    std::ostream(&_buf)
{
}

DigestOutputStream::DigestOutputStream(DigestEngine& eng, std::ostream& ostr):
    DigestIOS(eng, ostr),
    std::ostream(&_buf)
{
}

DigestOutputStream::~DigestOutputStream() {
}

void DigestOutputStream::close() {
    _buf.close();
}

} // namespace lyx;
