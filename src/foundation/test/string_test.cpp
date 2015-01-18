#include <gtest/gtest.h>
#include "lyxString.h"

using namespace lyx;

TEST(StringTest, string) {
    std::string str("aAbBcC");
    toLowerInPlace(str);

    EXPECT_EQ("aabbcc", str);
}
