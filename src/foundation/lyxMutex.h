#ifndef LIBLYX_FOUNDATION_LYXMUTEX_H_
#define LIBLYX_FOUNDATION_LYXMUTEX_H_

#include "lyxMutexImpl.h"
#include "lyxScopedLock.h"

namespace lyx {

/**
 * A Mutex (mutual exclusion) is a synchronization-
 * mechanism used to control access to a shared resource
 * in a concurrent (multithreaded) scenario.
 * Mutexes are recursive, that is, the same mutex can be-
 * locked multiple times by the same thread (but, of course,
 * not by other threads).
 * Using the ScopedLock class is the preferred way to automatically
 * lock and unlock a mutex.
 */
class Mutex: private MutexImpl {
    public:
        typedef lyx::ScopedLock<Mutex> ScopedLock;

        Mutex(bool fast = false);
        ~Mutex();

        /* Locks the mutex. Blocks if the mutex
         * is held by another thread.
         */
        void lock();

        /* Locks the mutex. Blocks up to the given number of milliseconds
         * if the mutex is held by another thread. Throws a Timeout Exception
         * if the mutex can not be locked within the given timeout.
         */
        void lock(long milliseconds);

        /* Tries to lock the mutex. Returns false immediately
         * if the mutex is already held by another thread.
         * Ruturns true if the mutex was successfully locked.
         */
        bool tryLock();

        /* Locks the mutex. Bolcks up to the given number of milliseconds
         * if the mutex is held by another thread.
         * Ruturns true if the mutex was successfully locked.
         */
        bool tryLock(long milliseconds);

        /* Unlocks the mutex so that it can be acquired by other threads.
         */
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
