#include "lyxHTTPBufferAllocator.h"
#include "lyxBugcheck.h"

namespace lyx {

MemoryPool HTTPBufferAllocator::_pool(HTTPBufferAllocator::BUFFER_SIZE, 16);

char* HTTPBufferAllocator::allocate(std::streamsize size) {
    lyx_assert_dbg (size == BUFFER_SIZE);

    return reinterpret_cast<char*>(_pool.get());
}

void HTTPBufferAllocator::deallocate(char* ptr, std::streamsize size) {
    lyx_assert_dbg (size == BUFFER_SIZE);

    _pool.release(ptr);
}


} // namespace lyx
