#include "lyxMemoryStream.h"

namespace lyx {

MemoryIOS::MemoryIOS(char* pBuffer, std::streamsize bufferSize):
    _buf(pBuffer, bufferSize)
{
}

MemoryIOS::~MemoryIOS() {
}

MemoryInputStream::MemoryInputStream(const char* pBuffer, std::streamsize bufferSize):
    MemoryIOS(const_cast<char*>(pBuffer), bufferSize),
    std::istream(&_buf)
{
}

MemoryInputStream::~MemoryInputStream() {
}

MemoryOutputStream::MemoryOutputStream(const char* pBuffer, std::streamsize bufferSize):
    MemoryIOS(const_cast<char*>(pBuffer), bufferSize),
    std::ostream(&_buf)
{
}

MemoryOutputStream::~MemoryOutputStream() {
}

} // namespace lyx
