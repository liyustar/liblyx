#include <gtest/gtest.h>

#include "lyxFormat.h"

using namespace lyx;

TEST(FormatTest, test) {
    EXPECT_EQ("3, hello:world", format("%d, %s:%s", 3, "hello", "world"));
}
