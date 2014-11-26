#include "lyxThreadImpl.h"
#include "lyxException.h"
#include "lyxTimespan.h"
#include "lyxTimestamp.h"
#include <signal.h>

namespace {

class SignalBlocker {
    public:
        SignalBlocker() {
            sigset_t sset;
            sigemptyset(&sset);
            sigaddset(&sset, SIGPIPE);
            pthread_sigmask(SIG_BLOCK, &sset, 0);
        }
        ~SignalBlocker() {
        }
};

static SignalBlocker signalBlocker;

}

namespace {

void setThreadName(pthread_t thread, const char* threadName) {
    pthread_setname_np(thread, threadName);
}

}

namespace lyx {

ThreadImpl::CurrentThreadHolder ThreadImpl::_currentThreadHolder;

ThreadImpl::ThreadImpl():
    _pData(new ThreadData)
{
}

ThreadImpl::~ThreadImpl() {
    if (_pData->started && !_pData->joined) {
        pthread_detach(_pData->thread);
    }
}

void ThreadImpl::setPriorityImpl(int prio) {
    if (prio != _pData->prio) {
        _pData->prio = prio;
        _pData->policy = SCHED_OTHER;
        if (isRunningImpl()) {
            struct sched_param par;
            par.sched_priority = mapPrio(_pData->prio, SCHED_OTHER);
            if (pthread_setschedparam(_pData->thread, SCHED_OTHER, &par))
                throw SystemException("cannot set thread priority");
        }
    }
}

void ThreadImpl::setOSPriorityImpl(int prio, int policy) {
    if (prio != _pData->osPrio || policy != _pData->policy) {
        if (_pData->pRunnableTarget || _pData->pCallbackTarget) {
            struct sched_param par;
            par.sched_priority = prio;
            if (pthread_setschedparam(_pData->thread, policy, &par))
                throw SystemException("cannot set thread priority");
        }
        _pData->prio   = reverseMapPrio(prio, policy);
        _pData->osPrio = prio;
        _pData->policy = policy;
    }
}

int ThreadImpl::getMinOSPriorityImpl(int policy) {
    return sched_get_priority_min(policy);
}

int ThreadImpl::getMaxOSPriorityImpl(int policy) {
    return sched_get_priority_max(policy);
}

void ThreadImpl::setStackSizeImpl(int size) {
    _pData->stackSize = 0;
}

int ThreadImpl::mapPrio(int prio, int policy) {
    int pmin = getMinOSPriorityImpl(policy);
    int pmax = getMaxOSPriorityImpl(policy);

    switch (prio) {
        case PRIO_LOWEST_IMPL:
            return pmin;
        case PRIO_LOW_IMPL:
            return pmin + (pmax - pmin)/4;
        case PRIO_NORMAL_IMPL:
            return pmin + (pmax - pmin)/2;
        case PRIO_HIGH_IMPL:
            return pmin + 3*(pmax - pmin)/4;
        case PRIO_HIGHEST_IMPL:
            return pmax;
        default:
            ;   // BUG: invalid thread priority
    }
    return -1;  // just to satisfy compiler - we'll never get here anyway
}

int ThreadImpl::reverseMapPrio(int prio, int policy) {
    if (policy == SCHED_OTHER) {
        int pmin = getMinOSPriorityImpl(policy);
        int pmax = getMaxOSPriorityImpl(policy);
        int normal = pmin + (pmax - pmin)/2;
        if (prio == pmax)
            return PRIO_HIGHEST_IMPL;
        else if (prio > normal)
            return PRIO_HIGH_IMPL;
        else if (prio == normal)
            return PRIO_NORMAL_IMPL;
        else if (prio > pmin)
            return PRIO_LOW_IMPL;
        else
            return PRIO_LOWEST_IMPL;
    }
    else
        return PRIO_HIGHEST_IMPL;
}

} // namespace lyx
