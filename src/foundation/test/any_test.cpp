#include <gtest/gtest.h>

#include "lyxAny.h"

using namespace lyx;

TEST(AnyTest, testDefaultCtor) {
    const Any value;

    EXPECT_TRUE(value.empty());
    EXPECT_EQ(0, AnyCast<int>(&value));
    EXPECT_EQ(typeid(void), value.type());
}
