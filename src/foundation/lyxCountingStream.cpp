#include "lyxCountingStream.h"

namespace lyx {

CountingStreamBuf::CountingStreamBuf():
    _chars(0)
{
}

CountingStreamBuf::~CountingStreamBuf() {
}

int CountingStreamBuf::writeToDevice(char c) {
    ++_chars;
    return charToInt(c);
}

CountingIOS::CountingIOS() {
}

CountingIOS::~CountingIOS() {
}

CountingStreamBuf* CountingIOS::rdbuf() {
    return &_buf;
}

CountingOutputStream::CountingOutputStream(): std::ostream(&_buf) {
}

CountingOutputStream::~CountingOutputStream() {
}

} // namespace lyx
