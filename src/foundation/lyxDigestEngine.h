#ifndef LIBLYX_FOUNDATION_LYXDIGESTENGINE_H_
#define LIBLYX_FOUNDATION_LYXDIGESTENGINE_H_

#include <string>
#include <vector>

namespace lyx {

/**
 * This class is an abstract base class
 * for all classes implementing a message
 * digest algorithm, like MD5Engine
 * and SHA1Engine.
 * Call update() repeatedly with data to
 * compute the digest from. When done,
 * call digest() to obtain the message
 * digest.
 */
class DigestEngine {
    public:
        typedef std::vector<unsigned char> Digest;

        DigestEngine();
        virtual ~DigestEngine();

        void update(const void* data, std::size_t length);
        void update(char data);
        void update(const std::string& data);

        virtual std::size_t digestLength() const = 0;

        virtual void reset() = 0;

        virtual const Digest& digest() = 0;

        static std::string digestToHex(const Digest& bytes);

        static Digest digestFromHex(const std::string& digest);

    protected:
        virtual void updateImpl(const void* data, std::size_t length) = 0;

    private:
        DigestEngine(const DigestEngine&);
        DigestEngine& operator = (const DigestEngine&);
};

inline void DigestEngine::update(const void* data, std::size_t length) {
    updateImpl(data, length);
}

inline void DigestEngine::update(char data) {
    updateImpl(&data, 1);
}

inline void DigestEngine::update(const std::string& data) {
    updateImpl(data.data(), data.size());
}

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXDIGESTENGINE_H_
