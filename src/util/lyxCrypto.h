#ifndef LIBLYX_UTIL_LYXCRYPTO_H_
#define LIBLYX_UTIL_LYXCRYPTO_H_

#include <string>

namespace lyx {

class Crypto {
    // 计算字符串MD5值,结果为16位二进制
    static std::string stringToMD5(const std::string& str);

    static std::string bytesToHex(const std::string& bytes);
    // static std::string hexToBytes(const std::string& str);
};

} // namespace lyx
#endif // LIBLYX_UTIL_LYXCRYPTO_H_
