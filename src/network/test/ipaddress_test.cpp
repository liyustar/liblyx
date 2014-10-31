#include "lyxIPAddress.h"
#include "gtest/gtest.h"

using namespace lyx;

TEST(IPAddressTest, StringConv) {
    // IPAddress ia1("127.0.0.1", IPAddress::IPv4); // Compile warning, Running error.
    IPAddress ia1("127.0.0.1");
    EXPECT_EQ(IPAddress::IPv4, ia1.family());
    EXPECT_STREQ("127.0.0.1", ia1.toString().c_str());
    EXPECT_EQ("127.0.0.1", ia1.toString());

    IPAddress ia2("192.168.1.120");
    EXPECT_EQ(IPAddress::IPv4, ia2.family());
    EXPECT_STREQ("192.168.1.120", ia2.toString().c_str());

    IPAddress ia3("255.255.255.255");
    EXPECT_EQ(IPAddress::IPv4, ia3.family());
    EXPECT_STREQ("255.255.255.255", ia3.toString().c_str());

    IPAddress ia4("0.0.0.0");
    EXPECT_EQ(IPAddress::IPv4, ia4.family());
    EXPECT_STREQ("0.0.0.0", ia4.toString().c_str());
}

TEST(IPAddressTest, Relationals) {
    IPAddress ip1("192.168.1.120");
    IPAddress ip2(ip1);
    IPAddress ip3;
    IPAddress ip4("10.0.0.138");

    EXPECT_TRUE(ip1 != ip4);
    EXPECT_TRUE(ip1 == ip2);
    EXPECT_TRUE(!(ip1 != ip2));
    EXPECT_TRUE(!(ip1 == ip4));
    EXPECT_TRUE(ip1 > ip4);
    EXPECT_TRUE(ip1 >= ip4);
    EXPECT_TRUE(ip4 < ip1);
    EXPECT_TRUE(ip4 <= ip1);
    EXPECT_TRUE(!(ip1 < ip4));
    EXPECT_TRUE(!(ip1 <= ip4));
    EXPECT_TRUE(!(ip4 > ip1));
    EXPECT_TRUE(!(ip4 >= ip1));

    ip3 = ip1;
    EXPECT_TRUE(ip1 == ip3);
    ip3 = ip4;
    EXPECT_TRUE(ip1 != ip3);
    EXPECT_TRUE(ip3 == ip4);

}

