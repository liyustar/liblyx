#ifndef LIBLYX_FOUNDATION_LYXSEMAPHOREIMPL_H_
#define LIBLYX_FOUNDATION_LYXSEMAPHOREIMPL_H_

#include "lyxException.h"
#include <pthread.h>

namespace lyx {

class SemaphoreImpl {
    protected:
        SemaphoreImpl(int n, int max);
        ~SemaphoreImpl();
        void setImpl();
        void waitImpl();
        bool waitImpl(long milliseconds);

    private:
        volatile int    _n;
        int             _max;
        pthread_mutex_t _mutex;
        pthread_cond_t _cond;
};

inline void SemaphoreImpl::setImpl() {
    if (pthread_mutex_lock(&_mutex))
        throw SystemException("cannot signal semaphore (lock)");

    if (_n < _max) {
        ++_n;
    }
    else {
        pthread_mutex_unlock(&_mutex);
        throw SystemException("cannot signal semaphore: count would exceed maximum");
    }
    if (pthread_cond_signal(&_cond)) {
        pthread_mutex_unlock(&_mutex);
        throw SystemException("cannot signal semaphore");
    }
    pthread_mutex_unlock(&_mutex);
}

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXSEMAPHOREIMPL_H_
