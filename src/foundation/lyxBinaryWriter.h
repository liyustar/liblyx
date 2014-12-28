#ifndef LIBLYX_FOUNDATION_LYXBINARYWRITER_H_
#define LIBLYX_FOUNDATION_LYXBINARYWRITER_H_

#include "lyxBuffer.h"
#include "lyxMemoryStream.h"
#include "lyxBugcheck.h"
#include <string>
#include <vector>

namespace lyx {

class BinaryWriter {
    public:
        enum StreamByteOrder {
            NATIVE_BYTE_ORDER       = 1,
            BIG_ENDIAN_BYTE_ORDER   = 2,
            NETWORK_BYTE_ORDER      = 2,
            LITTLE_ENDIANBYTE_ORDER = 3,
        };

        BinaryWriter(std::ostream& ostr, StreamByteOrder byteOrder = NATIVE_BYTE_ORDER);
        // BinaryWriter(std::ostream& ostr, TextEncoding& encoding, StreamByteOrder byteOrder = NATIVE_BYTE_ORDER);
        ~BinaryWriter();

        BinaryWriter& operator << (bool value);
        BinaryWriter& operator << (char value);
        BinaryWriter& operator << (unsigned char value);
        BinaryWriter& operator << (signed char value);
        BinaryWriter& operator << (short value);
        BinaryWriter& operator << (unsigned short value);
        BinaryWriter& operator << (int value);
        BinaryWriter& operator << (unsigned int value);
        BinaryWriter& operator << (long value);
        BinaryWriter& operator << (unsigned long value);
        BinaryWriter& operator << (float value);
        BinaryWriter& operator << (double value);
        BinaryWriter& operator << (long long value);
        BinaryWriter& operator << (unsigned long long value);
        BinaryWriter& operator << (const std::string& value);
        BinaryWriter& operator << (const char* value);

        template <typename T> BinaryWriter& operator << (const std::vector<T>& value) {
            uint32_t size(static_cast<uint32_t>(value.size()));

            *this << size;
            for (typename std::vector<T>::const_iterator it = value.begin(); it != value.end(); it++) {
                *this << *it;
            }

            return *this;
        }

        void write7BitEncoded(uint32_t value);
        void write7BitEncoded(uint64_t value);
        void writeRaw(const std::string& rawData);
        void writeRaw(const char* buffer, std::streamsize length);
        void writeBOM();
        void flush();
        bool good();
        bool fail();
        bool bad();

        std::ostream& stream() const;
        StreamByteOrder byteOrder() const;

    private:
        std::ostream&   _ostr;
        bool            _flipBytes;
        // TextConverter*  _pTextConverter;
};

template <typename T> class BasicMemoryBinaryWriter: public BinaryWriter {
    public:
        BasicMemoryBinaryWriter(const Buffer<T>& data, StreamByteOrder byteOrder = NATIVE_BYTE_ORDER):
            BinaryWriter(_ostr, byteOrder),
            _data(data),
            _ostr(data.begin(), data.capacity()) {
            }

        ~BasicMemoryBinaryWriter() {
            try {
                flush();
            }
            catch (...) {
                lyx_unexpected();
            }
        }

        Buffer<T>& data() {
            return _data;
        }

        const Buffer<T>& data() const {
            return _data;
        }

        const MemoryInputStream& stream() const {
            return _ostr;
        }

        MemoryInputStream& stream() {
            return _ostr;
        }

    private:
        Buffer<T>& _data;
        MemoryInputStream _ostr;
};

typedef BasicMemoryBinaryWriter<char> MemoryBinaryWriter;

inline bool BinaryWriter::good() {
    return _ostr.good();
}

inline bool BinaryWriter::fail() {
    return _ostr.fail();
}

inline bool BinaryWriter::bad() {
    return _ostr.bad();
}

inline std::ostream& BinaryWriter::stream() const {
    return _ostr;
}

inline BinaryWriter::StreamByteOrder BinaryWriter::byteOrder() const {
    return _flipBytes ? LITTLE_ENDIANBYTE_ORDER : BIG_ENDIAN_BYTE_ORDER;
}

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXBINARYWRITER_H_
