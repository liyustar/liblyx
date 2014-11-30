#ifndef LIBLYX_FOUNDATION_LYXTHREAD_H
#define LIBLYX_FOUNDATION_LYXTHREAD_H

#include "lyxThreadImpl.h"
#include "lyxEvent.h"
#include "lyxMutex.h"

namespace lyx {

class Runnabel;
class ThreadLocalStorage;

class Thread: private ThreadImpl {
    public:
        typedef ThreadImpl::TIDImpl TID;

        using ThreadImpl::Callable;

        enum Priority {
            PRIO_LOWEST  = PRIO_LOWEST_IMPL,
            PRIO_LOW     = PRIO_LOW_IMPL,
            PRIO_NORMAL  = PRIO_NORMAL_IMPL,
            PRIO_HIGH    = PRIO_HIGH_IMPL,
            PRIO_HIGHEST = PRIO_HIGHEST_IMPL
        };

        enum Polixy {
            POLICY_DEFAULT = POLICY_DEFAULT_IMPL
        };

        Thread();
        Thread(const std::string& name);
        ~Thread();

        int id() const;
        TID tid() const;
        std::string name() const;
        std::string getName() const;
        void setName(const std::string& name);

        void setPriority(Priority prio);
        Priority getPriority() const;
        void setOSPriority(int prio, int policy = POLICY_DEFAULT);
        int getOSPriority() const;

        static int getMinOSPriority(int policy = POLICY_DEFAULT);
        static int getMaxOSPriority(int policy = POLICY_DEFAULT);

        void setStackSize(int size);
        int  getStackSize() const;

        void start(Runnable& target);
        void start(Callable target, void* pData = 0);
        void join();
        void join(long milliseconds);
        bool tryJoin(long milliseconds);
        bool isRunning() const;
        static bool trySleep(long milliseconds);
        void wakeUp();
        static void sleep(long milliseconds);
        static void yield();
        static Thread* current();
        static TID currentTid();

    protected:
        ThreadLocalStorage& tls();
        void clearTLS();
        std::string makeName();
        static int uniqueId();

    private:
        Thread(const Thread&);
        Thread& operator = (const Thread&);

        int                 _id;
        std::string         _name;
        ThreadLocalStorage* _pTLS;
        Event               _event;
        mutable Mutex       _mutex;

        friend class ThreadLocalStorage;
        friend class PooledThread;
};

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXTHREAD_H
