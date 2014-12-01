#include "gtest/gtest.h"

#include "lyxSemaphore.h"
#include "lyxThread.h"
#include "lyxRunnable.h"
#include "lyxException.h"

using namespace lyx;

class SemaRunnable: public Runnable {
    public:
        SemaRunnable(int n, int max): _ran(false), _sema(n, max) {
        }

        void run() {
            _sema.wait();
            _ran = true;
        }

        bool ran() const {
            return _ran;
        }

        void set() {
            _sema.set();
        }

        void wait() {
            _sema.wait();
        }

        void wait(long milliseconds) {
            _sema.wait(milliseconds);
        }

        bool tryWait(long milliseconds) {
            return _sema.tryWait(milliseconds);
        }

    private:
        bool _ran;
        Semaphore _sema;
};

TEST(SemaphoreTest, InitZero) {
    SemaRunnable r(0, 3);
    EXPECT_TRUE(!r.tryWait(10));
    r.set();
    r.wait();
    try {
        r.wait(100);
        FAIL();   // must timeout
    }
    catch (TimeoutException&) {
    }
    catch (...) {
        FAIL();    // wrong exception
    }
    r.set();
    r.set();
    EXPECT_TRUE(r.tryWait(0));
    r.wait();
    EXPECT_TRUE(!r.tryWait(10));

    Thread t;
    t.start(r);
    Thread::sleep(100);
    EXPECT_TRUE(!r.ran());
    r.set();
    t.join();
    EXPECT_TRUE(r.ran());
    EXPECT_TRUE(!r.tryWait(10));
}

TEST(SemaphoreTest, InitNonZero) {
    SemaRunnable r(2, 2);
    r.wait();
    EXPECT_TRUE(r.tryWait(10));
    EXPECT_TRUE(!r.tryWait(10));
    r.set();
    EXPECT_TRUE(r.tryWait(10));
    EXPECT_TRUE(!r.tryWait(10));
}

TEST(SemaphoreTest, ExceedMax) {
    SemaRunnable r(2, 2);
    try {
        r.set();
    }
    catch (SystemException&) {
    }
    catch (...) {
        FAIL();
    }
}
