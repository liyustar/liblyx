#include "lyxThreadPool.h"
#include "lyxRunnable.h"
#include "lyxThread.h"
#include "lyxThreadLocal.h"
#include "lyxEvent.h"
#include <ctime>

namespace lyx {

class PooledThread: public Runnable {
    public:
        PooledThread(const std::string& name, int stackSize = 0);
        ~PooledThread();

        void start();
        bool idle();
        int  idleTime();
        void join();
        void activate();
        void release();
        void run();

    private:
        volatile bool           _idle;
        volatile std::time_t    _idleTime;
        Runnable*               _pTarget;
        std::string             _name;
        Thread                  _thread;
        Event                   _targetReady;
        Event                   _targetCompleted;
        Event                   _started;
        FastMutex               _mutex;
};

PooledThread::PooledThread(const std::string& name, int stackSize):
    _idle(true),
    _idleTime(0),
    _pTarget(0),
    _name(name),
    _thread(name),
    _targetCompleted(false)
{
    _thread.setStackSize(stackSize);
    _idleTime = std::time(NULL);
}

PooledThread::~PooledThread() {
}

void PooledThread::start() {
    _thread.start(*this);
    _started.wait();
}

bool PooledThread::idle() {
    return _idle;
}

int PooledThread::idleTime() {
    FastMutex::ScopedLock lock(_mutex);
    return (int) (std::time(NULL) - _idleTime);
}

void PooledThread::join() {
    _mutex.lock();
    Runnable* pThread = _pTarget;
    _mutex.unlock();
    if (pThread)
        _targetCompleted.wait();
}

void PooledThread::activate() {
    FastMutex::ScopedLock lock(_mutex);

    _idle = false;
    _targetCompleted.reset();
}

void PooledThread::release() {
    const long JOIN_TIMEOUT = 10000;

    _mutex.lock();
    _pTarget = 0;
    _mutex.unlock();

    // In case of a statically allocated thread pool (such
    // as the default thread pool), Windows may have already
    // terminated the thread before we got here.
    if (_thread.isRunning())
        _targetReady.set();

    if (_thread.tryJoin(JOIN_TIMEOUT))
        delete this;
}

void PooledThread::run() {
    _started.set();
    for (;;) {
        _targetReady.wait();
        _mutex.lock();
        if (_pTarget) { // a NULL target means kill yourself
            _mutex.unlock();
            // TODO: try {
            _pTarget->run();
            // TODO: } catch(...)
            FastMutex::ScopedLock lock(_mutex);
            _pTarget = 0;
            _idleTime = std::time(NULL);
            _idle = true;
            _targetCompleted.set();
            ThreadLocalStorage::clear();
            _thread.setName(_name);
            _thread.setPriority(Thread::PRIO_NORMAL);
        }
        else {
            _mutex.unlock();
            break;
        }
    }
}

} // namespace lyx
