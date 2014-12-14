#ifndef LIBLYX_FOUNDATION_LYXSHA1ENGINE_H_
#define LIBLYX_FOUNDATION_LYXSHA1ENGINE_H_

#include "lyxDigestEngine.h"
#include <openssl/sha.h>

namespace lyx {

class SHA1Engine: public DigestEngine {
    public:
        enum {
            DIGEST_SIZE = SHA_DIGEST_LENGTH
        };

        SHA1Engine();
        ~SHA1Engine();

        std::size_t digestLength() const;
        void reset();
        const DigestEngine::Digest& digest();

    protected:
        void updateImpl(const void* data, std::size_t length);

    private:
        SHA_CTX _context;
        DigestEngine::Digest _digest;

        SHA1Engine(const SHA1Engine&);
        SHA1Engine& operator = (const SHA1Engine&);
};

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXSHA1ENGINE_H_
