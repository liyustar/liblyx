#ifndef LIBLYX_FOUNDATION_LYXFILESTREAMBUF_H_
#define LIBLYX_FOUNDATION_LYXFILESTREAMBUF_H_

#include "lyxBufferedBidirectionalStreamBuf.h"
#include <string>
#include <istream>
#include <ostream>

namespace lyx {

class FileStreamBuf: public BufferedBindirectionalStreamBuf {
    public:
        FileStreamBuf();
        ~FileStreamBuf();

        void open( const std::string& path, std::ios::openmode mode);
        bool close();
        std::streampos seekoff(std::streamoff off, std::ios::seekdir dir, std::ios::openmode mode = std::ios::in | std::ios::out);
        std::streampos seekpos(std::streampos pos, std::ios::openmode mode = std::ios::in | std::ios::out);

    protected:
        enum {
            BUFFER_SIZE = 4096
        };

        int readFromDevice(char* buffer, std::streamsize length);
        int writeToDevice(const char* buffer, std::streamsize length);

    private:
        std::string _path;
        int _fd;
        std::streamoff _pos;
};

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXFILESTREAMBUF_H_
