#ifndef LIBLYX_FOUNDATION_LYXSCOPEDUNLOCK_H_
#define LIBLYX_FOUNDATION_LYXSCOPEDUNLOCK_H_

#include "lyxException.h"

namespace lyx {

/**
 * A class that simplifies thread synchronization
 * with a mutex.
 * The constructor accepts a Mutex and unlocks it.
 * The destructor locks the mutex.
 */
template <class M> class ScopedUnlock {
    public:
        inline ScopedUnlock(M& mutex, bool unlockNow = true): _mutex(mutex) {
            if (unlockNow)
                _mutex.unlock();
        }

        inline ~ScopedUnlock() {
            _mutex.lock();
        }

    private:
        M& _mutex;

        ScopedUnlock();
        ScopedUnlock(const ScopedUnlock&);
        ScopedUnlock& operator = (const ScopedUnlock&);
};

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXSCOPEDUNLOCK_H_
