#include "lyxSemaphoreImpl.h"
#include <sys/time.h>
#include <cassert>

namespace lyx {

SemaphoreImpl::SemaphoreImpl(int n, int max): _n(n), _max(max) {
    assert (n >= 0 && max > 0 && n <= max);

    if (pthread_mutex_init(&_mutex, NULL))
        throw SystemException("cannot create semaphore (mutex)");
    if (pthread_cond_init(&_cond, NULL))
        throw SystemException("cannot create semaphore (condition)");
}

SemaphoreImpl::~SemaphoreImpl() {
    pthread_cond_destroy(&_cond);
    pthread_mutex_destroy(&_mutex);
}

void SemaphoreImpl::waitImpl() {
    if (pthread_mutex_lock(&_mutex) != 0)
        throw SystemException("wait for semaphore failed (lock)");
    while (_n < 1) {
        if (pthread_cond_wait(&_cond, &_mutex)) {
            pthread_mutex_unlock(&_mutex);
            throw SystemException("wait for semaphore failed");
        }
    }
    --_n;
    pthread_mutex_unlock(&_mutex);
}

bool SemaphoreImpl::waitImpl(long milliseconds) {
    int rc = 0;
    struct timespec abstime;

    struct timeval tv;
    gettimeofday(&tv, NULL);
    abstime.tv_sec  = tv.tv_sec + milliseconds / 1000;
    abstime.tv_nsec = tv.tv_usec*1000 + (milliseconds % 1000)*1000000;
    if (abstime.tv_nsec >= 1000000000) {
        abstime.tv_nsec -= 1000000000;
        abstime.tv_sec++;
    }


    if (pthread_mutex_lock(&_mutex) != 0)
        throw SystemException("wait for semaphore failed (lock)");
    while (_n < 1) {
        if ((rc = pthread_cond_timedwait(&_cond, &_mutex, &abstime))) {
            if (rc == ETIMEDOUT) break;
            pthread_mutex_unlock(&_mutex);
            throw SystemException("wait for semaphore failed");
        }
    }
    if (rc == 0) --_n;
    pthread_mutex_unlock(&_mutex);
    return rc == 0;
}

} // namespace lyx
