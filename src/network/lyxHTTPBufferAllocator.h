#ifndef LIBLYX_NETWORK_LYXHTTPBUFFERALLOCATOR_H_
#define LIBLYX_NETWORK_LYXHTTPBUFFERALLOCATOR_H_

#include "lyxMemoryPool.h"
#include <ios>

namespace lyx {

/**
 * A BufferAllocator for HTTP streams.
 */
class HTTPBufferAllocator {
    public:
        static char* allocate(std::streamsize size);
        static void deallocate(char* ptr, std::streamsize size);

        enum {
            BUFFER_SIZE = 4096
        };

    private:
        static lyx::MemoryPool _pool;
};

} // namespace lyx

#endif // LIBLYX_NETWORK_LYXHTTPBUFFERALLOCATOR_H_
