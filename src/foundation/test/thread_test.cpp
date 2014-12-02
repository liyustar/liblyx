#include "gtest/gtest.h"

#include <string>

#include "lyxThread.h"
#include "lyxRunnable.h"
#include "lyxEvent.h"

using namespace lyx;

class MyRunnable: public Runnable {
    public:
        MyRunnable(): _ran(false) {
        }

        void run() {
            Thread* pThread = Thread::current();
            if (pThread)
                _threadName = pThread->name();
            _ran = true;
            _event.wait();
        }

        bool ran() const {
            return _ran;
        }

        const std::string& threadName() const {
            return _threadName;
        }

        void notify() {
            _event.set();
        }

        static void staticFunc() {
            ++_staticVar;
        }

        static int _staticVar;

    private:
        bool _ran;
        std::string _threadName;
        Event _event;
};

int MyRunnable::_staticVar = 0;

void freeFunc() {
    ++MyRunnable::_staticVar;
}

void freeFunc(void* pData) {
    MyRunnable::_staticVar += *reinterpret_cast<int*>(pData);
}

class NonJoinRunnable: public Runnable {
    public:
        NonJoinRunnable(): _finished(false) {
        }

        void run() {
            _finished = true;
        }

        bool finished() const {
            return _finished;
        }

    private:
        bool _finished;
};

class TrySleepRunnable: public Runnable {
    public:
        TrySleepRunnable(): _counter(0), _sleepy(true) {
        }

        void run() {
            _sleepy = !Thread::trySleep(300000);
            ++_counter;
            _sleepy = !Thread::trySleep(300000);
            ++_counter;
            _sleepy = !Thread::trySleep(100);
            ++_counter;
        }

        int counter() const {
            return _counter;
        }

        bool isSleepy() const {
            return _sleepy;
        }

    private:
        int _counter;
        bool _sleepy;
};

TEST(ThreadTest, Thread) {
    Thread thread;
    MyRunnable r;
    EXPECT_TRUE(!thread.isRunning());
    thread.start(r);
    Thread::sleep(200);
    EXPECT_TRUE(thread.isRunning());
    r.notify();
    thread.join();
    EXPECT_TRUE(!thread.isRunning());
    EXPECT_TRUE(r.ran());
    EXPECT_TRUE(!r.threadName().empty());
}
