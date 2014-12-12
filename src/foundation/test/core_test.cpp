#include <gtest/gtest.h>

#include "lyxBuffer.h"
#include "lyxException.h"

using namespace lyx;

TEST(CoreTest, Buffer) {
    std::size_t s = 10;
    Buffer<int> b(s);
    EXPECT_EQ(s, b.size());
    EXPECT_EQ(s * sizeof(int), b.sizeBytes());
    EXPECT_EQ(s, b.capacity());
    EXPECT_EQ(s * sizeof(int), b.capacityBytes());

    std::vector<int> v;
    for (unsigned int i = 0; i < s; i++)
        v.push_back(i);

    std::memcpy(b.begin(), &v[0], sizeof(int) * v.size());

    EXPECT_EQ(s, b.size());
    for (unsigned int i = 0; i < s; i++)
        EXPECT_EQ(i, b[i]);

    b.resize(s/2);
    for (unsigned int i = 0; i < s/2; i++)
        EXPECT_EQ(i, b[i]);

    EXPECT_EQ(s/2, b.size());
    EXPECT_EQ(s, b.capacity());

    b.resize(s*2);
    v.clear();
    for (unsigned int i = 0; i < s*2; i++)
        v.push_back(i);

    std::memcpy(b.begin(), &v[0], sizeof(int) * v.size());

    for (unsigned int i = 0; i < s*2; i++)
        EXPECT_EQ(i, b[i]);

    EXPECT_EQ(s*2, b.size());
    EXPECT_EQ(s*2, b.capacity());

    b.setCapacity(s * 4);
    EXPECT_EQ(s*2, b.size());
    EXPECT_EQ(s*4, b.capacity());

    b.setCapacity(s);
    EXPECT_EQ(s, b.size());
    EXPECT_EQ(s, b.capacity());

    Buffer<int> c(s);
    Buffer<int> d(c);
    EXPECT_TRUE(c == d);

    c[1] = -1;
    EXPECT_EQ(-1, c[1]);
    c.clear();
    EXPECT_EQ(0, c[1]);

    Buffer<int> e(0);
    EXPECT_TRUE(e.empty());

    EXPECT_TRUE(c != e);

    Buffer<int> f = e;
    EXPECT_TRUE(f == e);

    Buffer<char> g(0);
    g.append("hello", 5);
    EXPECT_EQ(5, g.size());

    g.append("hello", 5);
    EXPECT_EQ(10, g.size());
    EXPECT_EQ(0, std::memcmp(g.begin(), "hellohello", 10));

    char h[10];
    Buffer<char> buf(h, 10);
    try {
        buf.append("hello", 5);
        FAIL();
    }
    catch (InvalidAccessException&) {
    }

    buf.assign("hello", 5);
    EXPECT_EQ(0, std::memcmp(&h[0], "hello", 5));

    const char j[10] = "hello";
    Buffer<char> k(j, 5);
    k.append("hello", 5);
    EXPECT_EQ(0, std::memcmp(&j[0], "hello", 5));
    EXPECT_EQ(0, std::memcmp(k.begin(), "hellohello", 10));
    k.append('w');
    EXPECT_EQ(11, k.size());
    EXPECT_EQ(0, std::memcmp(k.begin(), "hellohellow", k.size()));
    k.append('o');
    EXPECT_EQ(12, k.size());
    EXPECT_EQ(0, std::memcmp(k.begin(), "hellohellowo", k.size()));
    k.append('r');
    EXPECT_EQ(13, k.size());
    EXPECT_EQ(0, std::memcmp(k.begin(), "hellohellowor", k.size()));
    k.append('l');
    EXPECT_EQ(14, k.size());
    EXPECT_EQ(0, std::memcmp(k.begin(), "hellohelloworl", k.size()));
    k.append('d');
    EXPECT_EQ(15, k.size());
    EXPECT_EQ(0, std::memcmp(k.begin(), "hellohelloworld", k.size()));
}
