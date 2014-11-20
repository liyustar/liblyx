#include "lyxTimestamp.h"
#include "gtest/gtest.h"
#include <unistd.h>

using namespace lyx;

TEST(TimestampTest, test) {
    Timestamp t1;
    sleep(2);
    Timestamp t2;
    Timestamp t3 = t2;
    EXPECT_TRUE(t1 != t2);
    EXPECT_TRUE(!(t1 == t2));
    EXPECT_TRUE(t2 > t1);
    EXPECT_TRUE(t2 >= t1);
    EXPECT_TRUE(!(t1 > t2));
    EXPECT_TRUE(!(t1 >= t2));
    EXPECT_TRUE(t2 == t3);
    EXPECT_TRUE(!(t2 != t3));
    EXPECT_TRUE(t2 >= t3);
    EXPECT_TRUE(t2 <= t3);
    Timestamp::TimeDiff d = (t2 - t1);
    EXPECT_TRUE(d >= 2000000 && d < 2001000);

    t1.swap(t2);
    EXPECT_TRUE(t1 > t2);
    t2.swap(t1);

    Timestamp::UtcTimeVal tv = t1.utcTime();
    Timestamp t4 = Timestamp::fromUtcTime(tv);
    EXPECT_TRUE(t1 == t4);

    Timestamp epoch(0);
    tv = epoch.utcTime();
    EXPECT_TRUE(tv >> 32 == 0x01b21dd2);
    EXPECT_TRUE((tv & 0xffffffff)  == 0x13814000);

    Timestamp now;
    sleep(1);
    EXPECT_TRUE(now.elapsed() >= 1000000);
    EXPECT_TRUE(now.isElapsed(1000000));
    EXPECT_FALSE(now.isElapsed(1050000));
}
