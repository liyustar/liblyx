#include "lyxThreadPool.h"
#include "lyxRunnable.h"
#include "lyxThread.h"
#include "lyxThreadLocal.h"
#include "lyxEvent.h"
#include <sstream>
#include <ctime>
#include <cassert>

namespace lyx {

class PooledThread: public Runnable {
    public:
        PooledThread(const std::string& name, int stackSize = 0);
        ~PooledThread();

        void start();
        void start(Thread::Priority priority, Runnable& target);
        void start(Thread::Priority priority, Runnable& target, const std::string& name);
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

void PooledThread::start(Thread::Priority priority, Runnable& target) {
    FastMutex::ScopedLock lock(_mutex);

    assert (_pTarget == 0);

    _pTarget = &target;
    _thread.setPriority(priority);
    _targetReady.set();
}

void PooledThread::start(Thread::Priority priority, Runnable& target, const std::string& name) {
    FastMutex::ScopedLock lock(_mutex);

    std::string fullName(name);
    if (name.empty()) {
        fullName = _name;
    }
    else {
        fullName.append(" (");
        fullName.append(_name);
        fullName.append(")");
    }
    _thread.setName(fullName);
    _thread.setPriority(priority);

    assert (_pTarget == 0);

    _pTarget = &target;
    _targetReady.set();
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

ThreadPool::ThreadPool(int minCapacity,
        int maxCapacity,
        int idleTime,
        int stackSize):
    _minCapacity(minCapacity),
    _maxCapacity(maxCapacity),
    _idleTime(idleTime),
    _serial(0),
    _age(0),
    _stackSize(stackSize)
{
    assert (minCapacity >= 1 && maxCapacity >= minCapacity && idleTime > 0);

    for (int i = 0; i < _minCapacity; i++) {
        PooledThread* pThread = createThread();
        _threads.push_back(pThread);
        pThread->start();
    }
}

ThreadPool::ThreadPool(const std::string& name,
        int minCapacity,
        int maxCapacity,
        int idleTime,
        int stackSize):
    _name(name),
    _minCapacity(minCapacity),
    _maxCapacity(maxCapacity),
    _idleTime(idleTime),
    _serial(0),
    _age(0),
    _stackSize(stackSize)
{
    assert (minCapacity >= 1 && maxCapacity >= minCapacity && idleTime > 0);

    for (int i = 0; i < _minCapacity; i++) {
        PooledThread* pThread = createThread();
        _threads.push_back(pThread);
        pThread->start();
    }
}

ThreadPool::~ThreadPool() {
    try {
        stopAll();
    }
    catch (...) {
        assert (0);
    }
}

void ThreadPool::addCapacity(int n) {
    FastMutex::ScopedLock lock(_mutex);

    assert (_maxCapacity + n >= _minCapacity);
    _maxCapacity += n;
    housekeep();
}

int ThreadPool::capacity() const {
    FastMutex::ScopedLock lock(_mutex);
    return _maxCapacity;
}

int ThreadPool::available() const {
    FastMutex::ScopedLock lock(_mutex);

    int count = 0;
    for (ThreadVec::const_iterator it = _threads.begin(); it != _threads.end(); ++it) {
        if ((*it)->idle()) ++count;
    }
    return (int) (count + _maxCapacity - _threads.size());
}

int ThreadPool::used() const {
    FastMutex::ScopedLock lock(_mutex);

    int count = 0;
    for (ThreadVec::const_iterator it = _threads.begin(); it != _threads.end(); ++it) {
        if (!(*it)->idle()) ++count;
    }
    return count;
}

int ThreadPool::allocated() const {
    FastMutex::ScopedLock lock(_mutex);

    return int(_threads.size());
}

void ThreadPool::start(Runnable& target) {
    getThread()->start(Thread::PRIO_NORMAL, target);
}

void ThreadPool::start(Runnable& target, const std::string& name) {
    getThread()->start(Thread::PRIO_NORMAL, target, name);
}

void ThreadPool::startWithPriority(Thread::Priority priority, Runnable& target) {
    getThread()->start(priority, target);
}

void ThreadPool::startWithPriority(Thread::Priority priority, Runnable& target, const std::string& name) {
    getThread()->start(priority, target, name);
}

void ThreadPool::stopAll() {
    FastMutex::ScopedLock lock(_mutex);

    for (ThreadVec::const_iterator it = _threads.begin(); it != _threads.end(); ++it) {
        (*it)->release();
    }
    _threads.clear();
}

void ThreadPool::joinAll() {
    FastMutex::ScopedLock lock(_mutex);

    for (ThreadVec::const_iterator it = _threads.begin(); it != _threads.end(); ++it) {
        (*it)->join();
    }
    housekeep();
}

void ThreadPool::collect() {
    FastMutex::ScopedLock lock(_mutex);

    housekeep();
};

void ThreadPool::housekeep() {
    _age = 0;
    if ((int)_threads.size() <= _minCapacity)
        return;

    ThreadVec idleThreads;
    ThreadVec expiredThreads;
    ThreadVec activeThreads;
    idleThreads.reserve(_threads.size());
    activeThreads.reserve(_threads.size());

    for (ThreadVec::iterator it = _threads.begin(); it != _threads.end(); it++) {
        if ((*it)->idle()) {
            if ((*it)->idleTime() < _idleTime)
                idleThreads.push_back(*it);
            else
                expiredThreads.push_back(*it);
        }
        else {
            activeThreads.push_back(*it);
        }
    }

    int n = (int) activeThreads.size();
    int limit = (int) idleThreads.size() + n;
    if (limit < _minCapacity)
        limit = _minCapacity;
    idleThreads.insert(idleThreads.end(), expiredThreads.begin(), expiredThreads.end());
    _threads.clear();
    for (ThreadVec::iterator it = idleThreads.begin(); it != idleThreads.end(); it++) {
        if (n < limit) {
            _threads.push_back(*it);
            n++;
        }
        else {
            (*it)->release();
        }
    }
    _threads.insert(_threads.end(), activeThreads.begin(), activeThreads.end());
}

PooledThread* ThreadPool::getThread() {
    FastMutex::ScopedLock lock(_mutex);

    if (++_age == 32)
        housekeep();

    PooledThread* pThread = 0;
    for (ThreadVec::iterator it = _threads.begin(); !pThread && it != _threads.end(); it++) {
        if ((*it)->idle())
            pThread = *it;
    }
    if (!pThread) {
        if ((int)_threads.size() < _maxCapacity) {
            pThread = createThread();
            try {
                pThread->start();
                _threads.push_back(pThread);
            }
            catch (...) {
                delete pThread;
                throw;
            }
        }
        else {
            throw NoThreadAvailableException();
        }
    }
    pThread->activate();
    return pThread;
}

PooledThread* ThreadPool::createThread() {
    std::ostringstream name;
    name << _name << "[#" << ++_serial << "]";
    return new PooledThread(name.str(), _stackSize);
}

class ThreadPoolSingletonHolder {
    public:
        ThreadPoolSingletonHolder() {
            _pPool = 0;
        }

        ~ThreadPoolSingletonHolder() {
            delete _pPool;
        }

        ThreadPool* pool() {
            FastMutex::ScopedLock lock(_mutex);

            if (!_pPool) {
                _pPool = new ThreadPool("default");
                // if (THREAD_STACK_SIZE > 0)
                //      _pPool->setStackSize(THREAD_STACK_SIZE);
            }
            return _pPool;
        }

    private:
        ThreadPool* _pPool;
        FastMutex   _mutex;
};

namespace {
static ThreadPoolSingletonHolder sh;
}

ThreadPool& ThreadPool::defaultPool() {
    return *sh.pool();
}

} // namespace lyx
