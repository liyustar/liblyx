#ifndef LIBLYX_FOUNDATION_LYXMD5ENGINE_H_
#define LIBLYX_FOUNDATION_LYXMD5ENGINE_H_

#include "lyxDigestEngine.h"
#include <openssl/md5.h>

namespace lyx {

class MD5Engine: public DigestEngine {
    public:
        enum {
            DIGEST_SIZE = MD5_DIGEST_LENGTH
        };

        MD5Engine();
        ~MD5Engine();

        std::size_t digestLength() const;
        void reset();
        const DigestEngine::Digest& digest();

    protected:
        void updateImpl(const void* data, std::size_t length);

    private:
        MD5_CTX _context;
        DigestEngine::Digest _digest;

        MD5Engine(const MD5Engine&);
        MD5Engine& operator = (const MD5Engine&);
};

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXMD5ENGINE_H_
