#include "lyxIPAddressImpl.h"
#include "gtest/gtest.h"

using namespace lyx;

TEST(IPAddressTest, StringConv) {
    IPv4AddressImpl ia1(IPv4AddressImpl::parse("127.0.0.1"));
    EXPECT_EQ(IPv4AddressImpl::IPv4, ia1.family());
    EXPECT_STREQ("127.0.0.1", ia1.toString().c_str());

    IPv4AddressImpl ia2(IPv4AddressImpl::parse("192.168.1.120"));
    EXPECT_EQ(IPv4AddressImpl::IPv4, ia2.family());
    EXPECT_STREQ("192.168.1.120", ia2.toString().c_str());

    IPv4AddressImpl ia3(IPv4AddressImpl::parse("255.255.255.255"));
    EXPECT_EQ(IPv4AddressImpl::IPv4, ia3.family());
    EXPECT_STREQ("255.255.255.255", ia3.toString().c_str());

    IPv4AddressImpl ia4(IPv4AddressImpl::parse("0.0.0.0"));
    EXPECT_EQ(IPv4AddressImpl::IPv4, ia4.family());
    EXPECT_STREQ("0.0.0.0", ia4.toString().c_str());
}
