#include <gtest/gtest.h>

#include "lyxSHA1Engine.h"

using namespace lyx;

TEST(SHA1EngineTest, testSHA1) {
    SHA1Engine engine;

    // test vectors from FIPS 180-1

    engine.update("abc");
    EXPECT_EQ("a9993e364706816aba3e25717850c26c9cd0d89d", DigestEngine::digestToHex(engine.digest()));

    engine.update("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq");
    EXPECT_EQ("84983e441c3bd26ebaae4aa1f95129e5e54670f1", DigestEngine::digestToHex(engine.digest()));

    for (int i = 0; i < 1000000; ++i)
        engine.update('a');
    EXPECT_EQ("34aa973cd4c4daa4f61eeb2bdbad27316534016f", DigestEngine::digestToHex(engine.digest()));
}

