#include <gtest/gtest.h>

#include "lyxThreadPool.h"
#include "lyxRunnableAdapter.h"
#include "lyxException.h"
#include "lyxThread.h"

using namespace lyx;

class ThreadPoolTest: public testing::Test {
    public:
        ThreadPoolTest(): _event(false) {
        }

        void count() {
            _event.wait();
            for (int i = 0; i < 10000; i++) {
                _mutex.lock();
                _count++;
                _mutex.unlock();
            }
        }

    protected:
        virtual void SetUp() {
            _event.reset();
            _count = 0;
        }

        lyx::FastMutex _mutex;
        lyx::Event _event;
        int _count;

    // private:
};

TEST_F(ThreadPoolTest, threadpool) {
    ThreadPool pool(2, 3, 3);
    pool.setStackSize(1);

    EXPECT_EQ(2, pool.allocated());
    EXPECT_EQ(0, pool.used());
    EXPECT_EQ(3, pool.capacity());
    EXPECT_EQ(3, pool.available());

    pool.addCapacity(1);
    EXPECT_EQ(2, pool.allocated());
    EXPECT_EQ(0, pool.used());
    EXPECT_EQ(4, pool.capacity());
    EXPECT_EQ(4, pool.available());

    RunnableAdapter<ThreadPoolTest> ra(*this, &ThreadPoolTest::count);

    pool.start(ra);
    EXPECT_EQ(2, pool.allocated());
    EXPECT_EQ(1, pool.used());
    EXPECT_EQ(4, pool.capacity());
    EXPECT_EQ(3, pool.available());

    pool.start(ra);
    EXPECT_EQ(2, pool.allocated());
    EXPECT_EQ(2, pool.used());
    EXPECT_EQ(4, pool.capacity());
    EXPECT_EQ(2, pool.available());

    pool.start(ra);
    EXPECT_EQ(3, pool.allocated());
    EXPECT_EQ(3, pool.used());
    EXPECT_EQ(4, pool.capacity());
    EXPECT_EQ(1, pool.available());

    pool.start(ra);
    EXPECT_EQ(4, pool.allocated());
    EXPECT_EQ(4, pool.used());
    EXPECT_EQ(4, pool.capacity());
    EXPECT_EQ(0, pool.available());

    try {
        pool.start(ra);
        FAIL();
    }
    catch (lyx::NoThreadAvailableException&) {

    }
    catch (...) {
        FAIL();
    }

    _event.set(); // go!!!
    pool.joinAll();

    EXPECT_EQ(40000, _count);
    EXPECT_EQ(4, pool.allocated());
    EXPECT_EQ(0, pool.used());
    EXPECT_EQ(4, pool.capacity());
    EXPECT_EQ(4, pool.available());

    Thread::sleep(4000);

    pool.collect();
    EXPECT_EQ(2, pool.allocated());
    EXPECT_EQ(0, pool.used());
    EXPECT_EQ(4, pool.capacity());
    EXPECT_EQ(4, pool.available());

    _count = 0;
    _event.reset();

    pool.start(ra);
    EXPECT_EQ(2, pool.allocated());
    EXPECT_EQ(1, pool.used());
    EXPECT_EQ(4, pool.capacity());
    EXPECT_EQ(3, pool.available());

    pool.start(ra);
    EXPECT_EQ(2, pool.allocated());
    EXPECT_EQ(2, pool.used());
    EXPECT_EQ(4, pool.capacity());
    EXPECT_EQ(2, pool.available());

    _event.set(); // go!!!
    pool.joinAll();

    EXPECT_EQ(20000, _count);
    EXPECT_EQ(2, pool.allocated());
    EXPECT_EQ(0, pool.used());
    EXPECT_EQ(4, pool.capacity());
    EXPECT_EQ(4, pool.available());
}
