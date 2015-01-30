#include "lyxLogFile.h"
#include "lyxLogFileImpl.h"

namespace lyx {

LogFile::LogFile(const std::string& path): LogFileImpl(path)
{
}

LogFile::~LogFile() {
}

} // namespace lyx
