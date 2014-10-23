#include "lyxDataStream.h"
#include <arpa/inet.h>

namespace lyx {

int DataStream::readInt(std::istream& istrm) {
    char buf[sizeof(int)];
    istrm.read(buf, sizeof(int));
    return ntohs(*reinterpret_cast<int*>(buf));
}

bool DataStream::writeInt(std::ostream& ostrm, int val) {
    int tmp = static_cast<int>(val);
    tmp = htons(tmp);
    ostrm.write(reinterpret_cast<char*>(&tmp), sizeof(int));
    return true;
}

std::string DataStream::readSizeAndBytes(std::istream& istrm) {
    const int BUFSIZE = 1024;
    char buf[BUFSIZE];
    istrm.read(buf, sizeof(int));
    int size = ntohs(*reinterpret_cast<int*>(buf));
    if (size > BUFSIZE) {
        return std::string();
    }
    istrm.read(buf, size);
    return std::string(buf, size);
}

bool DataStream::writeSizeAndBytes(std::ostream& ostrm, const std::string& str) {
    int size = static_cast<int>(str.length());
    size = htons(size);
    ostrm.write(reinterpret_cast<char*>(&size), sizeof(int));
    ostrm.write(str.c_str(), str.length());
    return true;
}

} // namespace lyx
