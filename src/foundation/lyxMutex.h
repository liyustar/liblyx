#ifndef LIBLYX_FOUNDATION_LYXMUTEX_H_
#define LIBLYX_FOUNDATION_LYXMUTEX_H_

#include "lyxMutexImpl.h"
#include "lyxScopedLock.h"

namespace lyx {

class Mutex: private MutexImpl {
    public:
        typedef lyx::ScopedLock<Mutex> ScopedLock;

        Mutex();
        ~Mutex();

        void lock();
        void lock(long milliseconds);
        bool tryLock();
        bool tryLock(long milliseconds);
        void unlock();

    private:
        Mutex(const Mutex&);
        Mutex& operator = (const Mutex&);
};

inline void Mutex::lock() {
    lockImpl();
}

inline void Mutex::lock(long milliseconds) {
    if (!tryLockImpl(milliseconds))
        throw TimeoutException();
}

inline bool Mutex::tryLock() {
    return tryLockImpl();
}

inline bool Mutex::tryLock(long milliseconds) {
    return tryLockImpl(milliseconds);
}

inline void Mutex::unlock() {
    unlockImpl();
}

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXMUTEX_H_
