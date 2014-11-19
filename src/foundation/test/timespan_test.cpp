#include "lyxTimespan.h"
#include "gtest/gtest.h"

using namespace lyx;

TEST(TimespanTest, Conversions) {
    Timespan ts;
    EXPECT_EQ(0, ts.totalMicroseconds());
    ts = Timespan::DAYS;
    EXPECT_EQ(Timespan::DAYS, ts.totalMicroseconds());
    EXPECT_EQ(86400000, ts.totalMilliseconds());
    EXPECT_EQ(86400, ts.totalSeconds());
    EXPECT_EQ(60*24, ts.totalMinutes());
    EXPECT_EQ(24, ts.totalHours());
    EXPECT_EQ(1, ts.days());

    EXPECT_EQ(0, ts.microseconds());
    EXPECT_EQ(0, ts.milliseconds());
    EXPECT_EQ(0, ts.seconds());
    EXPECT_EQ(0, ts.minutes());
    EXPECT_EQ(0, ts.hours());

    ts.assign(2, 12, 30, 10, 123456);
    EXPECT_EQ(456, ts.microseconds());
    EXPECT_EQ(123, ts.milliseconds());
    EXPECT_EQ(10, ts.seconds());
    EXPECT_EQ(30, ts.minutes());
    EXPECT_EQ(12, ts.hours());
    EXPECT_EQ(2, ts.days());

    ts.assign(0, 36, 30, 10, 123456);
    EXPECT_EQ(456, ts.microseconds());
    EXPECT_EQ(123, ts.milliseconds());
    EXPECT_EQ(123456, ts.useconds());
    EXPECT_EQ(10, ts.seconds());
    EXPECT_EQ(30, ts.minutes());
    EXPECT_EQ(12, ts.hours());
    EXPECT_EQ(1, ts.days());

    ts.assign(0, 0, 2190, 10, 123456);
    EXPECT_EQ(456, ts.microseconds());
    EXPECT_EQ(123, ts.milliseconds());
    EXPECT_EQ(123456, ts.useconds());
    EXPECT_EQ(10, ts.seconds());
    EXPECT_EQ(30, ts.minutes());
    EXPECT_EQ(12, ts.hours());
    EXPECT_EQ(1, ts.days());
}
