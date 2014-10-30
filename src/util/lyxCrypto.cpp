#include "lyxCrypto.h"
#include <cctype>   // for toupper()
#include <openssl/md5.h>
#include <openssl/sha.h>

namespace lyx {

static std::string byteToHex(uint8_t byte) {
    const char HEX[] = "0123456789ABCDEF";
    std::string hex;
    hex.append(1, HEX[byte >> 4]);
    hex.append(1, HEX[byte & 0x0F]);
    return hex;
}

std::string Crypto::stringToMD5(const std::string& str) {
    MD5_CTX md5_ctx;
    union {
        unsigned char uchar_arr[MD5_DIGEST_LENGTH];
        char char_arr[MD5_DIGEST_LENGTH];
    } md = {0};
    MD5_Init(&md5_ctx);
    MD5_Update(&md5_ctx, str.c_str(), str.length());
    MD5_Final(md.uchar_arr, &md5_ctx);
    return std::string(md.char_arr, MD5_DIGEST_LENGTH);
}

std::string Crypto::stringToSHA1(const std::string& str) {
    SHA_CTX sha_ctx;
    union {
        unsigned char   uchar_arr[SHA_DIGEST_LENGTH];
        char            char_arr[SHA_DIGEST_LENGTH];
    } md = {0};
    SHA1_Init(&sha_ctx);
    SHA1_Update(&sha_ctx, str.c_str(), str.length());
    SHA1_Final(md.uchar_arr, &sha_ctx);
    return std::string(md.char_arr, SHA_DIGEST_LENGTH);
}

std::string Crypto::bytesToHex(const std::string& bytes) {
    std::string hex;
    hex.reserve(bytes.length());
    for (int i = 0, len = bytes.length(); i < len; i++) {
        hex.append(byteToHex(bytes.at(i)));
    }
    return hex;
}

} // namespace lyx
