#include "gtest/gtest.h"

#include <string>

#include "lyxThread.h"
#include "lyxRunnable.h"
#include "lyxEvent.h"
#include "lyxTimestamp.h"
#include "lyxTimespan.h"
#include "lyxThreadTarget.h"

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

TEST(ThreadTest, NamedThread) {
    Thread thread("MyThread");
    MyRunnable r;
    thread.start(r);
    r.notify();
    thread.join();
    EXPECT_TRUE(r.ran());
    EXPECT_TRUE(r.threadName() == "MyThread");
}


TEST(ThreadTest, Current) {
    EXPECT_EQ(NULL, Thread::current());
}

TEST(ThreadTest, Threads) {
    Thread thread1("Thread1");
    Thread thread2("Thread2");
    Thread thread3("Thread3");
    Thread thread4("Thread4");

    MyRunnable r1;
    MyRunnable r2;
    MyRunnable r3;
    MyRunnable r4;
    EXPECT_TRUE(!thread1.isRunning());
    EXPECT_TRUE(!thread2.isRunning());
    EXPECT_TRUE(!thread3.isRunning());
    EXPECT_TRUE(!thread4.isRunning());

    thread1.start(r1);
    Thread::sleep(200);
    EXPECT_TRUE(thread1.isRunning());
    EXPECT_TRUE(!thread2.isRunning());
    EXPECT_TRUE(!thread3.isRunning());
    EXPECT_TRUE(!thread4.isRunning());

    thread2.start(r2);
    thread3.start(r3);
    thread4.start(r4);
    Thread::sleep(200);
    EXPECT_TRUE(thread1.isRunning());
    EXPECT_TRUE(thread2.isRunning());
    EXPECT_TRUE(thread3.isRunning());
    EXPECT_TRUE(thread4.isRunning());

    r4.notify();
    thread4.join();
    EXPECT_TRUE(!thread4.isRunning());
    EXPECT_TRUE(thread1.isRunning());
    EXPECT_TRUE(thread2.isRunning());
    EXPECT_TRUE(thread3.isRunning());

    r3.notify();
    thread3.join();
    EXPECT_TRUE(!thread3.isRunning());

    r2.notify();
    thread2.join();
    EXPECT_TRUE(!thread2.isRunning());

    r1.notify();
    thread1.join();
    EXPECT_TRUE(!thread1.isRunning());

    EXPECT_TRUE(r1.ran());
    EXPECT_EQ("Thread1", r1.threadName());
    EXPECT_TRUE(r2.ran());
    EXPECT_EQ("Thread2", r2.threadName());
    EXPECT_TRUE(r3.ran());
    EXPECT_EQ("Thread3", r3.threadName());
    EXPECT_TRUE(r4.ran());
    EXPECT_EQ("Thread4", r4.threadName());
}

TEST(ThreadTest, Join) {
    Thread thread;
    MyRunnable r;
    EXPECT_TRUE(!thread.isRunning());
    thread.start(r);
    Thread::sleep(200);
    EXPECT_TRUE(thread.isRunning());
    EXPECT_TRUE(!thread.tryJoin(100));
    r.notify();
    EXPECT_TRUE(thread.tryJoin(500));
    EXPECT_TRUE(!thread.isRunning());
}

TEST(ThreadTest, NotJoin) {
    Thread thread;
    TrySleepRunnable r;
    EXPECT_TRUE(!thread.isRunning());
    EXPECT_EQ(0, r.counter());
    EXPECT_TRUE(r.isSleepy());

    thread.start(r);
    EXPECT_TRUE(thread.isRunning());
    EXPECT_EQ(0, r.counter());
    EXPECT_TRUE(r.isSleepy());

    Thread::sleep(100);
    EXPECT_TRUE(thread.isRunning());
    EXPECT_EQ(0, r.counter());
    EXPECT_TRUE(r.isSleepy());

    thread.wakeUp(); Thread::sleep(10);
    EXPECT_TRUE(thread.isRunning());
    EXPECT_EQ(1, r.counter());
    EXPECT_TRUE(r.isSleepy());

    Thread::sleep(100);
    EXPECT_EQ(1, r.counter());

    thread.wakeUp(); Thread::sleep(10);
    EXPECT_EQ(2, r.counter());
    EXPECT_TRUE(r.isSleepy());

    Thread::sleep(200);
    EXPECT_EQ(3, r.counter());
    EXPECT_TRUE(!r.isSleepy());
    EXPECT_TRUE(!thread.isRunning());

    thread.wakeUp();
    EXPECT_TRUE(!thread.isRunning());
}

TEST(ThreadTest, NotRun) {
    Thread thread;
}

TEST(ThreadTest, NotRunJoin) {
    Thread thread;
    thread.join();
}

TEST(ThreadTest, ThreadTarget) {
    ThreadTarget te(&MyRunnable::staticFunc);
    Thread thread;

    EXPECT_TRUE(!thread.isRunning());

    int tmp = MyRunnable::_staticVar;
    thread.start(te);
    thread.join();
    EXPECT_EQ(MyRunnable::_staticVar, tmp + 1);

    ThreadTarget te1(freeFunc);
    EXPECT_TRUE(!thread.isRunning());

    tmp = MyRunnable::_staticVar;
    thread.start(te1);
    thread.join();
    EXPECT_EQ(MyRunnable::_staticVar, tmp + 1);
}

TEST(ThreadTest, ThreadFunction) {
    Thread thread;

    EXPECT_TRUE(!thread.isRunning());

    int tmp = MyRunnable::_staticVar;
    thread.start(freeFunc, &tmp);
    thread.join();
    EXPECT_EQ(MyRunnable::_staticVar, tmp * 2);

    EXPECT_TRUE(!thread.isRunning());

    tmp = MyRunnable::_staticVar = 0;
    thread.start(freeFunc, &tmp);
    thread.join();
    EXPECT_EQ(0, MyRunnable::_staticVar);
}

TEST(ThreadTest, ThreadStackSize) {
    int stackSize = 50000000;

    Thread thread;

    EXPECT_EQ(0, thread.getStackSize());
    thread.setStackSize(stackSize);
    EXPECT_TRUE(stackSize <= thread.getStackSize());
    int tmp = MyRunnable::_staticVar;
    thread.start(freeFunc, &tmp);
    thread.join();
    EXPECT_EQ(MyRunnable::_staticVar, tmp * 2);

    stackSize = 1;
    thread.setStackSize(stackSize);

    tmp = MyRunnable::_staticVar;
    thread.start(freeFunc, &tmp);
    thread.join();
    EXPECT_EQ(MyRunnable::_staticVar, tmp * 2);

    thread.setStackSize(0);
    EXPECT_EQ(0, thread.getStackSize());
    tmp = MyRunnable::_staticVar;
    thread.start(freeFunc, &tmp);
    thread.join();
    EXPECT_EQ(MyRunnable::_staticVar, tmp * 2);
}

TEST(ThreadTest, Sleep) {
    lyx::Timestamp start;
    Thread::sleep(200);
    lyx::Timespan elapsed = start.elapsed();
    EXPECT_TRUE(elapsed.totalMilliseconds() >= 190 && elapsed.totalMilliseconds() < 250);
}

