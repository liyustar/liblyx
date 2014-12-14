#include "lyxSHA1Engine.h"

namespace lyx {

SHA1Engine::SHA1Engine() {
    _digest.reserve(DIGEST_SIZE);
    SHA1_Init(&_context);
}

SHA1Engine::~SHA1Engine() {
}

void SHA1Engine::updateImpl(const void* input, std::size_t inputLen) {
    SHA1_Update(&_context, input, inputLen);
}

std::size_t SHA1Engine::digestLength() const {
    return DIGEST_SIZE;
}

void SHA1Engine::reset() {
    SHA1_Init(&_context);
}

const DigestEngine::Digest& SHA1Engine::digest() {
    unsigned char digest[DIGEST_SIZE];
    SHA1_Final(digest, &_context);
    _digest.clear();
    _digest.insert(_digest.begin(), digest, digest + sizeof(digest));
    return _digest;
}

} // namespace lyx
