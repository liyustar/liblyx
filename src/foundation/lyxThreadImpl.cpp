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

}
