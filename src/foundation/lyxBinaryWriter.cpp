#include "lyxBinaryWriter.h"
#include "lyxByteOrder.h"

namespace lyx {

BinaryWriter::BinaryWriter(std::ostream& ostr, StreamByteOrder byteOrder):
    _ostr(ostr)
    // _pTextConverter(0)
{
    _flipBytes = (byteOrder == LITTLE_ENDIANBYTE_ORDER);
}

BinaryWriter::~BinaryWriter() {
    // delete _pTextConverter;
}

BinaryWriter& BinaryWriter::operator << (bool value) {
    _ostr.write((const char*) &value, sizeof(value));
    return *this;
}

BinaryWriter& BinaryWriter::operator << (char value) {
    _ostr.write((const char*) &value, sizeof(value));
    return *this;
}

BinaryWriter& BinaryWriter::operator << (unsigned char value) {
    _ostr.write((const char*) &value, sizeof(value));
    return *this;
}

BinaryWriter& BinaryWriter::operator << (signed char value) {
    _ostr.write((const char*) &value, sizeof(value));
    return *this;
}

BinaryWriter& BinaryWriter::operator << (short value) {
    if (_flipBytes) value = ByteOrder::flipBytes(value);
    _ostr.write((const char*) &value, sizeof(value));
    return *this;
}

BinaryWriter& BinaryWriter::operator << (unsigned short value) {
    if (_flipBytes) value = ByteOrder::flipBytes(value);
    _ostr.write((const char*) &value, sizeof(value));
    return *this;
}

BinaryWriter& BinaryWriter::operator << (int value) {
    if (_flipBytes) value = ByteOrder::flipBytes(value);
    _ostr.write((const char*) &value, sizeof(value));
    return *this;
}

BinaryWriter& BinaryWriter::operator << (unsigned int value) {
    if (_flipBytes) value = ByteOrder::flipBytes(value);
    _ostr.write((const char*) &value, sizeof(value));
    return *this;
}

BinaryWriter& BinaryWriter::operator << (long value) {
    if (_flipBytes) value = ByteOrder::flipBytes((int) value);
    _ostr.write((const char*) &value, sizeof(value));
    return *this;
}

BinaryWriter& BinaryWriter::operator << (unsigned long value) {
    if (_flipBytes) value = ByteOrder::flipBytes((unsigned int) value);
    _ostr.write((const char*) &value, sizeof(value));
    return *this;
}

BinaryWriter& BinaryWriter::operator << (float value) {
    if (_flipBytes) {
        const char* ptr = (const char*) &value;
        ptr += sizeof(value);
        for (unsigned i = 0; i < sizeof(value); i++)
            _ostr.write(--ptr, 1);
    }
    else {
        _ostr.write((const char*) &value, sizeof(value));
    }
    return *this;
}

BinaryWriter& BinaryWriter::operator << (double value) {
    if (_flipBytes) {
        const char* ptr = (const char*) &value;
        ptr += sizeof(value);
        for (unsigned i = 0; i < sizeof(value); i++)
            _ostr.write(--ptr, 1);
    }
    else {
        _ostr.write((const char*) &value, sizeof(value));
    }
    return *this;
}

BinaryWriter& BinaryWriter::operator << (long long value) {
    if (_flipBytes) value = ByteOrder::flipBytes(value);
    _ostr.write((char*) &value, sizeof(value));
    return *this;
}

BinaryWriter& BinaryWriter::operator << (unsigned long long value) {
    if (_flipBytes) value = ByteOrder::flipBytes(value);
    _ostr.write((char*) &value, sizeof(value));
    return *this;
}

BinaryWriter& BinaryWriter::operator << (const std::string& value) {
    uint32_t length = (uint32_t) value.size();
    write7BitEncoded(length);
    _ostr.write(value.data(), length);
    return *this;
}

BinaryWriter& BinaryWriter::operator << (const char* value) {
    lyx_check_ptr (value);

    uint32_t length = static_cast<uint32_t>(std::strlen(value));
    write7BitEncoded(length);
    _ostr.write(value, length);
    return *this;
}

void BinaryWriter::write7BitEncoded(uint32_t value) {
    do {
        unsigned char c = (unsigned char) (value & 0x7F);
        value >>= 7;
        if (value) c |= 0x80;
        _ostr.write((const char*) &c, 1);
    } while (value);
}

void BinaryWriter::write7BitEncoded(uint64_t value) {
    do {
        unsigned char c = (unsigned char) (value & 0x7F);
        value >>= 7;
        if (value) c |= 0x80;
        _ostr.write((const char*) &c, 1);
    } while (value);
}

void BinaryWriter::writeRaw(const std::string& rawData) {
    _ostr.write(rawData.data(), (std::streamsize) rawData.length());
}

void BinaryWriter::writeRaw(const char* buffer, std::streamsize length) {
    _ostr.write(buffer, length);
}

void BinaryWriter::writeBOM() {
    uint16_t value = 0xFEFF;
    if (_flipBytes) value = ByteOrder::flipBytes(value);
    _ostr.write((const char*) &value, sizeof(value));
}

void BinaryWriter::flush() {
    _ostr.flush();
}

} // namespace lyx
