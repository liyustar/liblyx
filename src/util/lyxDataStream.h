#ifndef LIBLYX_UTIL_DATASTREAM_H_
#define LIBLYX_UTIL_DATASTREAM_H_

#include <iostream>
#include <string>

namespace lyx {

class DataStream {
    public:
        // read int
        static int readInt(std::istream& istrm);
        static bool writeInt(std::ostream& ostrm, int val);

        // read Size and Bytes
        static std::string readSizeAndBytes(std::istream& istrm);
        static bool writeSizeAndBytes(std::ostream& ostrm, const std::string& str);
};

} // namespace lyx

#endif // LIBLYX_UTIL_DATASTREAM_H_
