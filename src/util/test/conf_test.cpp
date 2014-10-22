#include "lyxConf.h"
#include "gtest/gtest.h"
#include <string>
#include <sstream>

using namespace lyx;

class ConfTest : public testing::Test {
    protected:
        virtual void SetUp() {
            conf_.setValue("SEC_1", "aa", "a");
            conf_.setValue("SEC_1", "bb", "b");

            conf_.setValue("SEC_2", "bb", "B");
            conf_.setValue("SEC_2", "cc", "C");

            conf_.setValue("SEC_1", "cc", "c");
            conf_.setValue("SEC_1", "dd", "d");
        }

        Conf conf_;
};

TEST_F(ConfTest, GetValue) {
    EXPECT_STREQ("a", conf_.getValue("SEC_1", "aa").c_str());
    EXPECT_STREQ("b", conf_.getValue("SEC_1", "bb").c_str());
    EXPECT_STREQ("c", conf_.getValue("SEC_1", "cc").c_str());
    EXPECT_STREQ("d", conf_.getValue("SEC_1", "dd").c_str());
    EXPECT_STREQ("B", conf_.getValue("SEC_2", "bb").c_str());
    EXPECT_STREQ("C", conf_.getValue("SEC_2", "cc").c_str());

    EXPECT_TRUE(conf_.getValue("SEC_1", "xx").empty());
    EXPECT_TRUE(conf_.getValue("SEC", "aa").empty());

    EXPECT_FALSE(conf_.modifyValue("SEC_1", "aa", "a"));
    EXPECT_TRUE(conf_.modifyValue("SEC_1", "aa", "change"));

    EXPECT_STREQ("change", conf_.getValue("SEC_1", "aa").c_str());
}

TEST_F(ConfTest, Clear) {
    EXPECT_TRUE(conf_.querySection("SEC_1"));
    EXPECT_TRUE(conf_.querySection("SEC_2"));
    EXPECT_FALSE(conf_.querySection("SEC_3"));

    EXPECT_TRUE(conf_.queryKey("SEC_1", "aa"));
    EXPECT_TRUE(conf_.queryKey("SEC_2", "cc"));

    conf_.clear();
    EXPECT_TRUE(conf_.empty());

    EXPECT_FALSE(conf_.querySection("SEC_2"));
    EXPECT_FALSE(conf_.queryKey("SEC_1", "aa"));
}

TEST_F(ConfTest, DumpStr) {
    std::ostringstream osstrm;
    osstrm << conf_;
    EXPECT_STREQ("[Not Implement]", osstrm.str().c_str());
}
