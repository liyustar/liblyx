#ifndef LIBLYX_FOUNDATION_LYXLOGFILEIMPL_H_
#define LIBLYX_FOUNDATION_LYXLOGFILEIMPL_H_

#include "lyxTimestamp.h"
#include "lyxFileStream.h"
#include <string>

namespace lyx {

class LogFileImpl {
    public:
        LogFileImpl(const std::string& path);
        ~LogFileImpl();
        void writeImpl(const std::string& text, bool flush);
        int sizeImpl() const;
        Timestamp creationDateImpl() const;
        const std::string& pathImpl() const;

    private:
        std::string _path;
        mutable FileOutputStream _str;
        Timestamp _creationDate;
};

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXLOGFILEIMPL_H_
