#ifndef LIBLYX_FOUNDATION_LYXTHREADIMPL_H
#define LIBLYX_FOUNDATION_LYXTHREADIMPL_H

#include <pthread.h>
#include <memory>
#include "lyxEvent.h"
#include "lyxRunnable.h"
#include "lyxSignalHandler.h"

namespace lyx {

class ThreadImpl {
    public:
        typedef pthread_t TIDImpl;
        typedef void (*Callable)(void*);

        enum {
            THREAD_STACK_SIZE = 0
        };

        enum Priority {
            PRIO_LOWEST_IMPL,
            PRIO_LOW_IMPL,
            PRIO_NORMAL_IMPL,
            PRIO_HIGH_IMPL,
            PRIO_HIGHEST_IMPL
        };

        enum Policy {
            POLICY_DEFAULT_IMPL = SCHED_OTHER
        };

        struct CallbackData {
            CallbackData(): callback(0), pData(0) {
            }

            Callable callback;
            void*    pData;
        };

        ThreadImpl();
        ~ThreadImpl();

        TIDImpl tidImpl() const;
        void setPriorityImpl(int prio);
        int  getPriorityImpl() const;
        void setOSPriorityImpl(int prio, int policy = SCHED_OTHER);
        int  getOSPriorityImpl() const;
        static int getMinOSPriorityImpl(int policy);
        static int getMaxOSPriorityImpl(int policy);
        void setStackSizeImpl(int size);
        int  getStackSizeImpl() const;
        void startImpl(Runnable& target);
        void startImpl(Callable target, void* pData = 0);

        void joinImpl();
        bool joinImpl(long milliseconds);
        bool isRunningImpl() const;
        static void sleepImpl(long milliseconds);
        static void yieldImpl();
        static ThreadImpl* currentImpl();
        static TIDImpl currentTidImpl();

    protected:
        static void* runnableEntry(void* pThread);
        static void* callableEntry(void* pThread);
        static int mapPrio(int prio, int policy = SCHED_OTHER);
        static int reverseMapPrio(int osPrio, int policy = SCHED_OTHER);

    private:
        class CurrentThreadHolder {
            public:
                CurrentThreadHolder() {
                    if (pthread_key_create(&_key, 0))
                        throw SystemException("cannot allocate thread context key");
                }
                ~CurrentThreadHolder() {
                    pthread_key_delete(_key);
                }
                ThreadImpl* get() const {
                    return reinterpret_cast<ThreadImpl*>(pthread_getspecific(_key));
                }
                void set(ThreadImpl* pThread) {
                    pthread_setspecific(_key, pThread);
                }

            private:
                pthread_key_t _key;
        };

        struct ThreadData {
            ThreadData():
                pRunnableTarget(0),
                pCallbackTarget(0),
                thread(0),
                prio(PRIO_NORMAL_IMPL),
                policy(SCHED_OTHER),
                done(false),
                stackSize(THREAD_STACK_SIZE),
                started(false),
                joined(false)
            {
            }

            Runnable*   pRunnableTarget;
            std::shared_ptr<CallbackData> pCallbackTarget;
            pthread_t   thread;
            int         prio;
            int         osPrio;
            int         policy;
            Event       done;
            std::size_t stackSize;
            bool        started;
            bool        joined;
        };

        std::shared_ptr<ThreadData> _pData;

        static CurrentThreadHolder _currentThreadHolder;

        SignalHandler::JumpBufferVec _jumpBufferVec;
        friend class SignalHandler;
};

inline int ThreadImpl::getPriorityImpl() const {
    return _pData->prio;
}

inline int ThreadImpl::getOSPriorityImpl() const {
    return _pData->osPrio;
}

inline bool ThreadImpl::isRunningImpl() const {
    return _pData->pRunnableTarget != 0 ||
        (_pData->pCallbackTarget.get() != 0 && _pData->pCallbackTarget->callback != 0);
}

inline void ThreadImpl::yieldImpl() {
    sched_yield();
}

inline int ThreadImpl::getStackSizeImpl() const {
    return static_cast<int>(_pData->stackSize);
}

inline ThreadImpl::TIDImpl ThreadImpl::tidImpl() const {
    return _pData->thread;
}

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXTHREADIMPL_H
