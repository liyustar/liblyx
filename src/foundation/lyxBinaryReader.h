#ifndef LIBLYX_FOUNDATION_LYXBINARYREADER_H_
#define LIBLYX_FOUNDATION_LYXBINARYREADER_H_

#include "lyxBuffer.h"
#include "lyxMemoryStream.h"
#include <string>
#include <vector>

namespace lyx {

class BinaryReader {
    public:
        enum StreamByteOrder {
            NATIVE_BYTE_ORDER       = 1,
            BIG_ENDIAN_BYTE_ORDER   = 2,
            NETWORK_BYTE_ORDER      = 2,
            LITTLE_ENDIANBYTE_ORDER = 3,
            UNSPECIFIED_BYTE_ORDER  = 4
        };

        BinaryReader(std::istream& istr, StreamByteOrder byteOrder = NATIVE_BYTE_ORDER);
        BinaryReader(std::istream& istr, /* TextEncoding& encoding,*/ StreamByteOrder byteOrder = NATIVE_BYTE_ORDER);
        ~BinaryReader();

        BinaryReader& operator >> (bool& value);
        BinaryReader& operator >> (char& value);
        BinaryReader& operator >> (unsigned char& value);
        BinaryReader& operator >> (signed char& value);
        BinaryReader& operator >> (short& value);
        BinaryReader& operator >> (unsigned short& value);
        BinaryReader& operator >> (int& value);
        BinaryReader& operator >> (unsigned int& value);
        BinaryReader& operator >> (long& value);
        BinaryReader& operator >> (unsigned long& value);
        BinaryReader& operator >> (float& value);
        BinaryReader& operator >> (double& value);
        BinaryReader& operator >> (long long& value);
        BinaryReader& operator >> (unsigned long long& value);
        BinaryReader& operator >> (std::string& value);

        template <typename T> BinaryReader& operator >> (std::vector<T>& value) {
            uint32_t size(0);
            T elem;

            *this >> size;
            if (!good()) return *this;
            value.reserve(size);
            while (this->good() && size-- > 0) { 
                *this >> elem;
                value.push_back(elem);
            }
            return *this;
        }

        void read7BitEncoded(uint32_t& value);
        void read7BitEncoded(uint64_t& value);
        void readRaw(std::streamsize length, std::string& value);
        void readRaw(char* buffer, std::streamsize length);
        void readBOM();
        bool good();
        bool fail();
        bool bad();
        bool eof();

        std::istream& stream() const;
        void setExceptions(std::ios_base::iostate st = (std::istream::failbit | std::istream::badbit));
        std::streamsize available() const;

    private:
        std::istream&   _istr;
        bool            _flipBytes;
        // TextConverter*  _pTextConverter;
};

template <typename T> class BasicMemoryBinaryReader: public BinaryReader {
    public:
        BasicMemoryBinaryReader(const Buffer<T>& data, StreamByteOrder byteOrder = NATIVE_BYTE_ORDER):
            BinaryReader(_istr, byteOrder),
            _data(data),
            _istr(data.begin(), data.capacity()) {
            }

        BasicMemoryBinaryReader(const Buffer<T>& data, /* TextEncoding& encoding,*/ StreamByteOrder byteOrder = NATIVE_BYTE_ORDER):
            BinaryReader(_istr, /*encoding,*/ byteOrder),
            _data(data),
            _istr(data.begin(), data.capacity()) {
            }

        ~BasicMemoryBinaryReader() {
        }

        const Buffer<T>& data() const {
            return _data;
        }

        const MemoryInputStream& stream() const {
            return _istr;
        }

        MemoryInputStream& stream() {
            return _istr;
        }

    private:
        const Buffer<T>& _data;
        MemoryInputStream _istr;
};

typedef BasicMemoryBinaryReader<char> MemoryBinaryReader;

inline bool BinaryReader::good() {
    return _istr.good();
}

inline bool BinaryReader::fail() {
    return _istr.fail();
}

inline bool BinaryReader::bad() {
    return _istr.bad();
}

inline bool BinaryReader::eof() {
    return _istr.eof();
}

inline std::istream& BinaryReader::stream() const {
    return _istr;
}

inline void BinaryReader::setExceptions(std::ios_base::iostate st) {
    _istr.exceptions(st);
}

inline std::streamsize BinaryReader::available() const {
    return _istr.rdbuf()->in_avail();
}

} // namespace lyx
#endif // LIBLYX_FOUNDATION_LYXBINARYREADER_H_
