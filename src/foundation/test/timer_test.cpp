#include <gtest/gtest.h>

#include "lyxTimer.h"
#include "lyxThread.h"
#include "lyxStopwatch.h"
#include "lyxEvent.h"

using namespace lyx;

class TimerTest: public testing::Test {
    public:
        void onTimer(Timer& t) {
            t.getStartInterval();
            _event.set();
        }

    protected:
        Event _event;
};

TEST_F(TimerTest, timer) {
    Timer t(100, 200);
    EXPECT_EQ(100, t.getStartInterval());
    EXPECT_EQ(200, t.getPeriodicInterval());

    /*
    Stopwatch sw;
    TimerCallback<TimerTest> tc(*this, &TimerTest::onTimer);

    sw.start();
    t.start(tc);

    _event.wait();
    sw.stop();
    EXPECT_TRUE(sw.elapsed() >= 80000 && sw.elapsed() < 120000);

    sw.restart();
    _event.wait();
    sw.stop();
    EXPECT_TRUE(sw.elapsed() >= 180000 && sw.elapsed() < 250000);

    sw.restart();
    _event.wait();
    sw.stop();
    EXPECT_TRUE(sw.elapsed() >= 180000 && sw.elapsed() < 250000);
    t.stop();
    */
}

TEST_F(TimerTest, DuplicateStop) {
    Timer t(100, 200);
    t.stop();
    t.stop();

    TimerCallback<TimerTest> tc(*this, &TimerTest::onTimer);
    t.start(tc);
    _event.wait();
    t.stop();
    t.stop();
}
