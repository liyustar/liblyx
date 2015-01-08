#ifndef LIBLYX_FOUNDATION_LYXFILE_H_
#define LIBLYX_FOUNDATION_LYXFILE_H_

#include <vector>
#include <cstddef>
#include "lyxMutex.h"

namespace lyx {

class MemoryPool {
    public:
        MemoryPool(std::size_t blockSize, int preAlloc = 0, int maxAlloc = 0);
        ~MemoryPool();

        void* get();
        void release(void* ptr);
        std::size_t blockSize() const;
        int allocated() const;
        int available() const;

    private:
        MemoryPool();
        MemoryPool(const MemoryPool&);
        MemoryPool& operator = (const MemoryPool&);

        enum {
            BLOCK_RESERVE = 128
        };

        typedef std::vector<char*> BlockVec;

        std::size_t _blockSize;
        int         _maxAlloc;
        int         _allocated;
        BlockVec    _blocks;
        FastMutex   _mutex;
};

inline std::size_t MemoryPool::blockSize() const {
    return _blockSize;
}

inline int MemoryPool::allocated() const {
    return _allocated;
}

inline int MemoryPool::available() const {
    return (int) _blocks.size();
}

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXFILE_H_
