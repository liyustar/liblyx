#ifndef LIBLYX_FOUNDATION_LYXBYTEORDER_H_
#define LIBLYX_FOUNDATION_LYXBYTEORDER_H_

namespace lyx {

class ByteOrder {
    public:
        typedef long long int64_t;
        typedef unsigned long long uint64_t;

        static int16_t  flipBytes(int16_t value);
        static uint16_t flipBytes(uint16_t value);
        static int32_t  flipBytes(int32_t value);
        static uint32_t flipBytes(uint32_t value);
        static int64_t  flipBytes(int64_t value);
        static uint64_t flipBytes(uint64_t value);
};

inline int16_t ByteOrder::flipBytes(int16_t value) {
    return int16_t(flipBytes(uint16_t(value)));
}

inline uint16_t ByteOrder::flipBytes(uint16_t value) {
    return ((value >> 8) & 0x00FF) | ((value << 8) & 0xFF00);
}

inline int32_t ByteOrder::flipBytes(int32_t value) {
    return int32_t(flipBytes(uint32_t(value)));
}

inline uint32_t ByteOrder::flipBytes(uint32_t value) {
    return ((value >> 24) & 0x000000FF) | ((value >> 8) & 0x0000FF00)
        | ((value << 8) & 0x00FF0000) | ((value << 24) & 0xFF000000);
}

inline ByteOrder::int64_t ByteOrder::flipBytes(int64_t value) {
    return int64_t(flipBytes(uint64_t(value)));
}

inline ByteOrder::uint64_t ByteOrder::flipBytes(uint64_t value) {
    uint32_t hi = uint32_t(value >> 32);
    uint32_t lo = uint32_t(value & 0xFFFFFFFF);
    return uint64_t(flipBytes(hi) | (uint64_t(flipBytes(lo)) << 32));
}

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXBYTEORDER_H_
