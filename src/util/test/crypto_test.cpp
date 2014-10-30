#include "lyxCrypto.h"
#include "gtest/gtest.h"

using namespace lyx;

TEST(CryptoTest, MD5) {
    EXPECT_EQ("5F4DCC3B5AA765D61D8327DEB882CF99",
            Crypto::bytesToHex(Crypto::stringToMD5("password")));
    EXPECT_EQ("202CB962AC59075B964B07152D234B70",
            Crypto::bytesToHex(Crypto::stringToMD5("123")));

}
