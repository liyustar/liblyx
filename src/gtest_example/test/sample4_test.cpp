#include "sample4.h"
#include "gtest/gtest.h"

TEST(Counter, Increment) {
    Counter c;

    EXPECT_EQ(0, c.Increment());
    EXPECT_EQ(1, c.Increment());
    EXPECT_EQ(2, c.Increment());
}
