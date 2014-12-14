#include <gtest/gtest.h>

#include "lyxMD5Engine.h"

using namespace lyx;

TEST(MD5EngineTest, MD5) {
    MD5Engine engine;

    // test vectors from RFC 1321

    engine.update("");
    EXPECT_EQ("d41d8cd98f00b204e9800998ecf8427e", DigestEngine::digestToHex(engine.digest()));

    engine.update("a");
    EXPECT_EQ("0cc175b9c0f1b6a831c399e269772661", DigestEngine::digestToHex(engine.digest()));

    engine.update("abc");
    EXPECT_EQ("900150983cd24fb0d6963f7d28e17f72", DigestEngine::digestToHex(engine.digest()));

    engine.update("message digest");
    EXPECT_EQ("f96b697d7cb7938d525a2f31aaf161d0", DigestEngine::digestToHex(engine.digest()));

    engine.update("abcdefghijklmnopqrstuvwxyz");
    EXPECT_EQ("c3fcd3d76192e4007dfb496cca67e13b", DigestEngine::digestToHex(engine.digest()));

    engine.update("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    engine.update("abcdefghijklmnopqrstuvwxyz0123456789");
    EXPECT_EQ("d174ab98d277d9f5a5611c2c9f419d9f", DigestEngine::digestToHex(engine.digest()));

    engine.update("12345678901234567890123456789012345678901234567890123456789012345678901234567890");
    EXPECT_EQ("57edf4a22be3c955ac49da2e2107b67a", DigestEngine::digestToHex(engine.digest()));
}
