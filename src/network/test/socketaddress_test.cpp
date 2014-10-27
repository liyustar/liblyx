#include "lyxSocketAddress.h"
#include "gtest/gtest.h"

using namespace lyx;

TEST(SocketAddressTest, Constructor) {
    SocketAddress wild;

    SocketAddress sa1("192.168.1.100", 100);
    EXPECT_EQ("192.168.1.100", sa1.host().toString());
    EXPECT_EQ(100, sa1.port());

    SocketAddress sa2("8.8.8.8", 53);
    EXPECT_EQ("8.8.8.8", sa2.host().toString());
    EXPECT_EQ(53, sa2.port());

    SocketAddress sa3("192.168.1.100", "ftp");
    EXPECT_EQ("192.168.1.100", sa3.host().toString());
    EXPECT_EQ(21, sa3.port());

    SocketAddress sa4("192.168.1.100", 100);
    EXPECT_EQ("192.168.1.100", sa4.host().toString());
    EXPECT_EQ(100, sa4.port());
}
