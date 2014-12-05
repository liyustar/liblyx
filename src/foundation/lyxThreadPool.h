#ifndef LIBLYX_FOUNDATION_LYXTHREADPOOL_H_
#define LIBLYX_FOUNDATION_LYXTHREADPOOL_H_

#include "lyxThread.h"
#include "lyxMutex.h"
#include <string>
#include <vector>

namespace lyx {

class Runnable;
class PooledThread;

/**
 * A thread pool always keeps a number of threads running, ready
 * to accept work.
 * Creating and starting a threads can impose a significant runtime
 * overhead to an application. A thread pool helps to improve
 * the performance of an application by reducing the number
 * of threads that have to be created (and destoryed again).
 * Threads in a thread pool are re-used once they become
 * available again.
 * The thread pool always keeps a minimum number of threads
 * running. If the demans for threads increases, additional
 * threads are created. Once the demand for threads sinks
 * again, no-longer used threads are stopped and removed
 * from the pool.
 */
class ThreadPool {
    public:
        ThreadPool(int minCapacity = 2,
                int maxCapacity = 16,
                int idleTime = 60,
                int stackSize = 0);

        ThreadPool(const std::string& name,
                int minCapacity = 2,
                int maxCapacity = 16,
                int idleTime = 60,
                int stackSize = 0);

        ~ThreadPool();

        void addCapacity(int n);
        void capacity() const;

        void setStackSize(int stackSize);
        int  getStackSize() const;

        int used() const;
        int allocated() const;
        int available() const;

        void start(Runnable& target);
        void start(Runnable& target, const std::string& name);

        void stopAll();
        void joinAll();
        void collect();

        const std::string& name() const;

        static ThreadPool& defaultPool();

    protected:
        PooledThread* getThread();
        PooledThread* createThread();

        void housekeep();

    private:
        ThreadPool(const ThreadPool& pool);
        ThreadPool& operator = (const ThreadPool& pool);

        typedef std::vector<PooledThread*> ThreadVec;

        std::string _name;
        int _minCapacity;
        int _maxCapacity;
        int _idleTime;
        int _serial;
        int _age;
        int _stackSize();
        ThreadVec _threads;
        mutable FastMutex _mutex;
};

inline void ThreadPool::setStackSize(int stackSize) {
    _stackSize = stackSize;
}

inline int ThreadPool::getStackSize() const {
    return _stackSize;
}

inline const std::string& ThreadPool::name() const {
    return _name;
}

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXTHREADPOOL_H_
