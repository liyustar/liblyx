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

TEST(AnyTest, testCastToReference) {
    Any a(137);
    const Any b(a);

    int&                ra    = AnyCast<int &>(a);
    int const&          ra_c  = AnyCast<int const &>(a);
    int volatile&       ra_v  = AnyCast<int volatile &>(a);
    int const volatile& ra_cv = AnyCast<int const volatile &>(a);

    // cv references to same obj
    EXPECT_TRUE(&ra == &ra_c && &ra == &ra_v && &ra == &ra_cv);

    int const &          rb_c  = AnyCast<int const &>(b);
    int const volatile & rb_cv = AnyCast<int const volatile &>(b);

    EXPECT_EQ(&rb_c, &rb_cv); // cv references to copied const obj
    EXPECT_NE(&ra, &rb_c); // copies hold different objects

    ++ra;
    int incremented = AnyCast<int>(a);
    EXPECT_EQ(138, incremented);    // increment by reference changes value

    try {
        AnyCast<char &>(a);
        FAIL(); // AnyCast to incorrent reference type
    }
    catch (BadCastException&) {
    }

    try {
        AnyCast<const char &>(b);
        FAIL(); // AnyCast to incorrent const reference type
    }
    catch (BadCastException&) {
    }
}

TEST(AnyTest, testBadCast) {
    std::string text = "test message";
    Any value = text;

    try {
        AnyCast<const char *>(value);
        FAIL(); // must throw
    }
    catch (BadCastException&) {
    }
}

TEST(AnyTest, testSwap) {
    std::string text = "test message";
    Any original = text, swapped;
    std::string* originalPtr = AnyCast<std::string>(&original);
    Any* swapResult = &original.swap(swapped);

    EXPECT_TRUE(original.empty());
    EXPECT_FALSE(swapped.empty());
    EXPECT_EQ(typeid(std::string), swapped.type());
    EXPECT_EQ(text, AnyCast<std::string>(swapped));
    EXPECT_NE(static_cast<std::string*>(0), originalPtr);
    EXPECT_EQ(swapResult, &original);
}

TEST(AnyTest, testEmptyCopy) {
    const Any null;
    Any copied = null, assigned;
    assigned = null;

    EXPECT_TRUE(null.empty());
    EXPECT_TRUE(copied.empty());
    EXPECT_TRUE(assigned.empty());
}

TEST(AnyTest, testInt) {
    Any e;
    EXPECT_TRUE(e.empty());

    Any a = 13;
    EXPECT_EQ(typeid(int), a.type());
    int* i = AnyCast<int>(&a);
    EXPECT_EQ(13, *i);
    Any b = a;
    EXPECT_EQ(typeid(int), b.type());
    int *cpyI = AnyCast<int>(&b);
    EXPECT_EQ(*cpyI, *i);
    *cpyI = 20;
    EXPECT_NE(*cpyI, *i);
    std::string* s = AnyCast<std::string>(&a);
    EXPECT_EQ(NULL, s);

    int tmp = AnyCast<int>(a);
    const Any c = a;
    tmp = AnyCast<int>(a);
    (void)tmp; // 消除 unused 警告
}

class SomeClass {
    public:
        int i;
        std::string str;
        SomeClass(int h, std::string s): i(h), str(s) {};
        bool operator == (const SomeClass& other) const {
            return i == other.i && str == other.str;
        }
};

TEST(AnyTest, testComplexType) {
    SomeClass str(13, std::string("hello"));
    Any a = str;
    Any b = a;
    EXPECT_EQ(typeid(SomeClass), a.type());
    EXPECT_EQ(typeid(SomeClass), b.type());
    SomeClass str2 = AnyCast<SomeClass>(a);
    EXPECT_EQ(str, str2);
    const SomeClass& strCRef = RefAnyCast<SomeClass>(a);
    EXPECT_EQ(str, strCRef);
    SomeClass& strRef = RefAnyCast<SomeClass>(a);
    EXPECT_EQ(str, strRef);
}

TEST(AnyTest, testVector) {
    std::vector<int> tmp;
    tmp.push_back(1);
    tmp.push_back(2);
    tmp.push_back(3);

    Any a = tmp;
    EXPECT_EQ(typeid(std::vector<int>), a.type());

    std::vector<int> tmp2 = AnyCast<std::vector<int> >(a);
    EXPECT_EQ(3, tmp2.size());

    const std::vector<int>& vecCRef = RefAnyCast<std::vector<int> >(a);
    std::vector<int>& vecRef = RefAnyCast<std::vector<int> >(a);

    EXPECT_EQ(1, vecRef[0]);
    EXPECT_EQ(2, vecRef[1]);
    EXPECT_EQ(3, vecRef[2]);
    vecRef[0] = 0;
    EXPECT_EQ(vecRef[0], vecCRef[0]);

}
