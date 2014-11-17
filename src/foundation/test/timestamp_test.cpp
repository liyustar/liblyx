#include "lyxTimestamp.h"
#include "gtest/gtest.h"
#include <unistd.h>

using namespace lyx;

TEST(TimestampTest, test) {
    Timestamp t1;
    Timestamp::UtcTimeVal tv = t1.utcTime();
    Timestamp t2 = Timestamp::fromUtcTime(tv);
    EXPECT_TRUE(t1 == t2);

    Timestamp now;
    sleep(1);
    EXPECT_TRUE(now.elapsed() >= 1000000);
    EXPECT_TRUE(now.isElapsed(1000000));
    EXPECT_FALSE(now.isElapsed(1050000));
}
