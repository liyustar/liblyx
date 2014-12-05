#ifndef LIBLYX_FOUNDATION_LYXMUTEXIMPL_H_
#define LIBLYX_FOUNDATION_LYXMUTEXIMPL_H_

#include <pthread.h>
#include <errno.h>
#include "lyxException.h"

namespace lyx {

class MutexImpl {
    protected:
        MutexImpl(bool fast = false);
        ~MutexImpl();
        void lockImpl();
        bool tryLockImpl();
        bool tryLockImpl(long milliseconds);
        void unlockImpl();

    private:
        pthread_mutex_t _mutex;
};

class FastMutexImpl: public MutexImpl {
    protected:
        FastMutexImpl();
        ~FastMutexImpl();
};

inline void MutexImpl::lockImpl() {
    if (pthread_mutex_lock(&_mutex))
        throw SystemException("cannot lock mutex");
}

inline bool MutexImpl::tryLockImpl() {
    int rc = pthread_mutex_trylock(&_mutex);
    if (rc == 0)
        return true;
    else if (rc == EBUSY)
        return false;
    else
        throw SystemException("cannot lock mutex");
}

inline void MutexImpl::unlockImpl() {
    if (pthread_mutex_unlock(&_mutex))
        throw SystemException("cannot unlock mutex");
}

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXMUTEXIMPL_H_
