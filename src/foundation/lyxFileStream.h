#ifndef LIBLYX_FOUNDATION_LYXFILESTREAM_H_
#define LIBLYX_FOUNDATION_LYXFILESTREAM_H_

#include "lyxFileStreamBuf.h"

namespace lyx {

class FileIOS: public virtual std::ios {
    public:
        FileIOS(std::ios::openmode defaultMode);
        ~FileIOS();

        void open(const std::string& path, std::ios::openmode mode);
        void close();

        FileStreamBuf* rdbuf();

    protected:
        FileStreamBuf _buf;
        std::ios::openmode _defaultMode;
};

class FileInputStream: public FileIOS, public std::istream {
    public:
        FileInputStream();
        FileInputStream(const std::string& paht, std::ios::openmode mode = std::ios::in);
        ~FileInputStream();
};

class FileOutputStream: public FileIOS, public std::ostream {
    public:
        FileOutputStream();
        FileOutputStream(const std::string& path, std::ios::openmode mode = std::ios::out | std::ios::trunc);
        ~FileOutputStream();
};

class FileStream: public FileIOS, public std::iostream {
    public:
        FileStream();
        FileStream(const std::string& path, std::ios::openmode mode = std::ios::out | std::ios::in);
        ~FileStream();
};

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXFILESTREAM_H_
