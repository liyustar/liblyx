#include <gtest/gtest.h>

#include "lyxThreadLocal.h"
#include "lyxThread.h"
#include "lyxRunnable.h"
#include <string>

using namespace lyx;

class TLTestRunnable: public Runnable {
    public:
        TLTestRunnable(int n): _n(n) {
        }

        void run() {
            *_count = 0;
            for (int i = 0; i < _n; i++) {
                (*_count)++;
            }
            _result = *_count;
        }

        int result() {
            return _result;
        }

    protected:
        int _n;
        int _result;
        static ThreadLocal<int> _count;
};

struct TLTestStruct {
    int i;
    std::string s;
};

ThreadLocal<int> TLTestRunnable::_count;

TEST(ThreadLocalTest, Locality) {
    TLTestRunnable r1(5000);
    TLTestRunnable r2(7500);
    TLTestRunnable r3(6000);

    Thread t1;
    Thread t2;
    Thread t3;

    t1.start(r1);
    t2.start(r2);
    t3.start(r3);

    t1.join();
    t2.join();
    t3.join();

    EXPECT_EQ(5000, r1.result());
    EXPECT_EQ(7500, r2.result());
    EXPECT_EQ(6000, r3.result());

    /*
    t1.start(r2);
    t1.join();
    EXPECT_EQ(12500, r1.result());
    */
}

TEST(ThreadLocalTest, Accessors) {
    ThreadLocal<TLTestStruct> ts;
    ts->i = 100;
    ts->s = "foo";
    EXPECT_EQ(100, (*ts).i);
    EXPECT_EQ("foo", (*ts).s);
    EXPECT_EQ(100, ts.get().i);
    EXPECT_EQ("foo", ts.get().s);
}
