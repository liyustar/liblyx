#include "lyxSocketAddress.h"
#include "gtest/gtest.h"

using namespace lyx;

TEST(SocketAddressTest, Constructor) {
    SocketAddress wild;

    SocketAddress sa1("192.168.1.100", 100);
    // sa1.host();
}
