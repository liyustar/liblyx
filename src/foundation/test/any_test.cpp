#include <gtest/gtest.h>

#include "lyxAny.h"

using namespace lyx;

TEST(AnyTest, testDefaultCtor) {
    const Any value;

    EXPECT_TRUE(value.empty());
    EXPECT_EQ(0, AnyCast<int>(&value));
    EXPECT_EQ(typeid(void), value.type());
}

TEST(AnyTest, testConvertingCtro) {
    std::string text = "test message";
    Any value = text;

    EXPECT_TRUE(!value.empty());
    EXPECT_EQ(typeid(std::string), value.type());
    EXPECT_EQ(0, AnyCast<int>(&value));
    EXPECT_NE(static_cast<std::string*>(0), AnyCast<std::string>(&value));
    EXPECT_EQ(text, AnyCast<std::string>(value));
    EXPECT_NE(&text, AnyCast<std::string>(&value));
}

TEST(AnyTest, testCopyCtor) {
    std::string text = "test message";
    Any original = text, copy = original;

    EXPECT_FALSE(copy.empty());
    EXPECT_EQ(original.type(), copy.type());
    EXPECT_EQ(AnyCast<std::string>(original), AnyCast<std::string>(copy));
    EXPECT_EQ(text, AnyCast<std::string>(copy));
    EXPECT_NE(AnyCast<std::string>(&original), AnyCast<std::string>(&copy));
}

TEST(AnyTest, testCopyAssign) {
    std::string text = "test message";
    Any original = text, copy;
    Any* assignResult = &(copy = original);

    EXPECT_FALSE(copy.empty());
    EXPECT_EQ(original.type(), copy.type());
    EXPECT_EQ(AnyCast<std::string>(original), AnyCast<std::string>(copy));
    EXPECT_EQ(text, AnyCast<std::string>(copy));
    EXPECT_NE(AnyCast<std::string>(&original), AnyCast<std::string>(&copy));
    EXPECT_EQ(&copy, assignResult);

    // test self assignment
    Any& ref = original;
    original = ref;
    EXPECT_EQ(AnyCast<std::string>(original), AnyCast<std::string>(copy));
    original = original;
    EXPECT_EQ(AnyCast<std::string>(original), AnyCast<std::string>(copy));
}

TEST(AnyTest, testConvertingAssign) {
    std::string text = "test message";
    Any value;
    Any* assignResult = &(value = text);

    EXPECT_FALSE(value.empty());
    EXPECT_EQ(typeid(std::string), value.type());
    EXPECT_EQ(0, AnyCast<int>(&value));
    EXPECT_NE(static_cast<std::string*>(0), AnyCast<std::string>(&value));
    EXPECT_EQ(text, AnyCast<std::string>(value));
    EXPECT_NE(&text, AnyCast<std::string>(&value));
    EXPECT_EQ(&value, assignResult);
}
