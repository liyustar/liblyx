#ifndef LIBLYX_FOUNDATION_LYXSCOPEDLOCK_H_
#define LIBLYX_FOUNDATION_LYXSCOPEDLOCK_H_

#include "lyxException.h"

namespace lyx {

/**
 * A class that simplifies thread synchronization
 * with a mutex.
 * The constructor accepts a Mutex (and optionally
 * a timeout value in milliseconds) and locks it.
 * The destructor unlocks the mutex.
 */
template <class M> class ScopedLock {
    public:
        explicit ScopedLock(M& mutex): _mutex(mutex) {
            _mutex.lock();
        }

        ScopedLock(M& mutex, long milliseconds): _mutex(mutex) {
            _mutex.lock(milliseconds);
        }

        ~ScopedLock() {
            _mutex.unlock();
        }

    private:
        M& _mutex;

        ScopedLock();
        ScopedLock(const ScopedLock&);
        ScopedLock& operator = (const ScopedLock&);
};

/**
 * A class that simplifies thread synchronization
 * with a mutex.
 * The constructor accepts a Mutex (and optionally
 * a timeout value in milliseconds) and locks it.
 * The destructor unlocks the mutex.
 * The unlock() member function allows for manual
 * unlocking of the mutex.
 */
template <class M> class ScopedLockWithUnlock {
    public:
        explicit ScopedLockWithUnlock(M& mutex): _pMutex(&mutex) {
            _pMutex->lock();
        }

        ScopedLockWithUnlock(M& mutex, long milliseconds): _pMutex(&mutex) {
            _pMutex->lock(milliseconds);
        }

        ~ScopedLockWithUnlock() {
            unlock();
        }

        void unlock() {
            if (_pMutex) {
                _pMutex->unlock();
                _pMutex = NULL;
            }
        }

    private:
        M* _pMutex;

        ScopedLockWithUnlock();
        ScopedLockWithUnlock(const ScopedLockWithUnlock&);
        ScopedLockWithUnlock& operator = (const ScopedLockWithUnlock&);
};

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXSCOPEDLOCK_H_
