#include "lyxMD5Engine.h"

namespace lyx {

MD5Engine::MD5Engine() {
    _digest.reserve(DIGEST_SIZE);
    reset();
}

MD5Engine::~MD5Engine() {
}

void MD5Engine::updateImpl(const void* input, std::size_t inputLen) {
    MD5_Update(&_context, input, inputLen);
}

std::size_t MD5Engine::digestLength() const {
    return DIGEST_SIZE;
}

void MD5Engine::reset() {
    MD5_Init(&_context);
}

const DigestEngine::Digest& MD5Engine::digest() {
    unsigned char digest[DIGEST_SIZE];
    MD5_Final(digest, &_context);
    _digest.clear();
    _digest.insert(_digest.begin(), digest, digest + sizeof(digest));
    reset();
    return _digest;
}

} // namespace lyx
