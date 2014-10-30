#include "lyxCrypto.h"
#include "gtest/gtest.h"

using namespace lyx;

TEST(CryptoTest, MD5) {
    EXPECT_EQ("5F4DCC3B5AA765D61D8327DEB882CF99",
            Crypto::bytesToHex(Crypto::stringToMD5("password")));
    EXPECT_EQ("202CB962AC59075B964B07152D234B70",
            Crypto::bytesToHex(Crypto::stringToMD5("123")));

}

TEST(CryptoTest, SHA1) {
    EXPECT_EQ("5BAA61E4C9B93F3F0682250B6CF8331B7EE68FD8",
            Crypto::bytesToHex(Crypto::stringToSHA1("password")));
    EXPECT_EQ("40BD001563085FC35165329EA1FF5C5ECBDBBEEF",
            Crypto::bytesToHex(Crypto::stringToSHA1("123")));

}
