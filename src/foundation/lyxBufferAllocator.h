#ifndef LIBLYX_FOUNDATION_LYXBUFFERALLOCATOR_H_
#define LIBLYX_FOUNDATION_LYXBUFFERALLOCATOR_H_

#include <ios>
#include <cstddef>

namespace lyx {

template <typename ch> class BufferAllocator {
    public:
        typedef ch char_type;

        static char_type* allocate(std::streamsize size) {
            return new char_type[static_cast<std::size_t>(size)];
        }

        static void deallocate(char_type* ptr, std::streamsize /*size*/) throw() {
            delete [] ptr;
        }
};

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXBUFFERALLOCATOR_H_
