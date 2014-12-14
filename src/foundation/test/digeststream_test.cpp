#include <gtest/gtest.h>

#include "lyxDigestStream.h"
#include "lyxMD5Engine.h"
#include <sstream>

using namespace lyx;

TEST(DigestStreamTest, InputStream) {
    std::istringstream istr("abcdefghijklmnopqrstuvwxyz");
    MD5Engine md5;
    DigestInputStream ds(md5, istr);
    std::string s;
    ds >> s;
    EXPECT_EQ("c3fcd3d76192e4007dfb496cca67e13b", DigestEngine::digestToHex(md5.digest()));
    EXPECT_EQ("abcdefghijklmnopqrstuvwxyz", s);
}

TEST(DigestStreamTest, OutputStream1) {
    MD5Engine md5;
    DigestOutputStream ds(md5);
    ds << "abcdefghijklmnopqrstuvwxyz";
    ds.close();
    EXPECT_EQ("c3fcd3d76192e4007dfb496cca67e13b", DigestEngine::digestToHex(md5.digest()));

    ds << "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    ds << "abcdefghijklmnopqrstuvwxyz0123456789";
    ds.close();
    EXPECT_EQ("d174ab98d277d9f5a5611c2c9f419d9f", DigestEngine::digestToHex(md5.digest()));
}

TEST(DigestStreamTest, OutputStream2) {
    MD5Engine md5;
    std::ostringstream ostr;
    DigestOutputStream ds(md5, ostr);
    ds << "abcdefghijklmnopqrstuvwxyz";
    ds.close();
    EXPECT_EQ("c3fcd3d76192e4007dfb496cca67e13b", DigestEngine::digestToHex(md5.digest()));
    EXPECT_EQ("abcdefghijklmnopqrstuvwxyz", ostr.str());
}

TEST(DigestStreamTest, ToFromHex) {
    std::string digest("c3fcd3d76192e4007dfb496cca67e13b");
    DigestEngine::Digest dig = DigestEngine::digestFromHex(digest);
    std::string digest2 = DigestEngine::digestToHex(dig);
    EXPECT_EQ(digest2, digest);
}
