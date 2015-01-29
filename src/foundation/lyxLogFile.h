#ifndef LIBLYX_FOUNDATION_LYXLOGFILE_H_
#define LIBLYX_FOUNDATION_LYXLOGFILE_H_

#include "lyxLogFileImpl.h"

namespace lyx {

class LogFile: public LogFileImpl {
    public:
        LogFile(const std::string& path);
        ~LogFile();

        void write(const std::string& text, bool flush = true);
        int size() const;
        Timestamp creationDate() const;
        const std::string& path() const;
};

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXLOGFILE_H_
