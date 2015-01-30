#include "lyxFileChannel.h"

#include "lyxMessage.h"

namespace lyx {

FileChannel::FileChannel():
    _times("utc"),
    _flush(true)
{
}

FileChannel::FileChannel(const std::string& path):
    _path(path),
    _times("utc"),
    _flush(true)
{
}

FileChannel::~FileChannel() {
}

void FileChannel::open() {
    FastMutex::ScopedLock lock(_mutex);

    if (!_pFile) {
        _pFile = new LogFile(_path);
    }
}

void FileChannel::close() {
    FastMutex::ScopedLock lock(_mutex);

    delete _pFile;
    _pFile = 0;
}

void FileChannel::log(const Message& msg) {
    open();

    FastMutex::ScopedLock lock(_mutex);

    _pFile->write(msg.getText(), _flush);
}

void FileChannel::setProperty(const std::string& name, const std::string& value) {
}

std::string FileChannel::getProperty(const std::string& name) const {
    return std::string();
}

} // namespace lyx
