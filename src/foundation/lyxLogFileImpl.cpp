#include "lyxLogFileImpl.h"

#include "lyxFile.h"
#include "lyxException.h"

namespace lyx {

LogFileImpl::LogFileImpl(const std::string& path):
    _path(path),
    _str(_path, std::ios::app)
{
    if (sizeImpl() == 0)
        _creationDate = File(path).getLastModified();
    else
        _creationDate = File(path).created();
}

LogFileImpl::~LogFileImpl() {
}

void LogFileImpl::writeImpl(const std::string& text, bool flush) {
    _str << text;
    if (flush)
        _str << std::endl;
    else
        _str << "\n";
    if (!_str.good())
        throw WriteFileException(_path);
}

int LogFileImpl::sizeImpl() const {
    return (int) _str.tellp();
}

Timestamp LogFileImpl::creationDateImpl() const {
    return _creationDate;
}

const std::string& LogFileImpl::pathImpl() const {
    return _path;
}

} // namespace lyx
