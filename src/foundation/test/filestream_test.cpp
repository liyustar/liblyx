#include <gtest/gtest.h>

#include "lyxFileStream.h"

using namespace lyx;

TEST(FileStreamTest, Read) {
    std::string file("testfile.txt");

    FileOutputStream fos(file, std::ios::binary);
    fos << "sometestdata";
    fos.close();

    FileInputStream fis(file);
    EXPECT_TRUE(fis.good());
    std::string read;
    fis >> read;
    EXPECT_TRUE(!read.empty());
    EXPECT_EQ("sometestdata", read);
}

TEST(FileStreamTest, Write) {
    std::string file("dummy_file.txt");

    FileOutputStream fos(file);
    EXPECT_TRUE(fos.good());
    fos << "hiho";
    fos.close();

    FileInputStream fis(file);
    EXPECT_TRUE(fis.good());
    std::string read;
    fis >> read;
    EXPECT_EQ("hiho", read);
}

TEST(FileStreamTest, ReadWrite) {
    char tmp[] = {'\xc3', '\x9f', '\xc3', '\x84', '\xc3', '\x96', '\xc3', '\x9c', '\xc3', '\xa4', '\xc3', '\xb6', '\xc3', '\xbc', '\0'};
    std::string file(tmp);
    file = "dummy_" + file + ".txt";

    FileStream fos(file);
    EXPECT_TRUE(fos.good());
    fos << "hiho";
    fos.seekg(0, std::ios::beg);
    std::string read;
    fos >> read;
    EXPECT_EQ("hiho", read);
}

TEST(FileStreamTest, Open) {
    FileOutputStream ostr;
    ostr.open("test.txt", std::ios::out);
    EXPECT_TRUE(ostr.good());
    ostr.close();
}

TEST(FileStreamTest, OpenModeAte) {
    FileOutputStream ostr("test.txt");
    ostr << "0123456789";
    ostr.close();

    FileStream str1("test.txt", std::ios::ate);
    int c = str1.get();
    EXPECT_TRUE(str1.eof());

    str1.clear();
    str1.seekg(0);
    c = str1.get();
    EXPECT_EQ('0', c);

    str1.close();

    FileStream str2("test.txt", std::ios::ate);
    str2 << "abcedf";
    str2.seekg(0);
    std::string s;
    str2 >> s;
    EXPECT_EQ("0123456789abcedf", s);
    str2.close();
}

TEST(FileStreamTest, OpenModeApp) {
    FileStream str("test.txt", std::ios::trunc);
    str << "0123456789abcedf";

    str.seekg(0);
    int c = str.get();
    EXPECT_EQ('0', c);

    str.seekg(10);
    EXPECT_EQ(std::streampos(10), str.tellg());
    c = str.get();
    EXPECT_EQ('a', c);
    EXPECT_EQ(std::streampos(11), str.tellg());

    str.seekg(-1, std::ios::end);
    EXPECT_EQ(std::streampos(15), str.tellg());
    c = str.get();
    EXPECT_EQ('f', c);
    EXPECT_EQ(std::streampos(16), str.tellg());

    str.seekg(-1, std::ios::cur);
    EXPECT_EQ(std::streampos(15), str.tellg());
    c = str.get();
    EXPECT_EQ('f', c);
    EXPECT_EQ(std::streampos(16), str.tellg());

    str.seekg(-4, std::ios::cur);
    EXPECT_EQ(std::streampos(12), str.tellg());
    c = str.get();
    EXPECT_EQ('c', c);
    EXPECT_EQ(std::streampos(13), str.tellg());

    str.seekg(1, std::ios::cur);
    EXPECT_EQ(std::streampos(14), str.tellg());
    c = str.get();
    EXPECT_EQ('d', c);
    EXPECT_EQ(std::streampos(15), str.tellg());
}

TEST(FileStreamTest, MultiOpen) {
    FileStream str("test.txt", std::ios::trunc);
    str << "0123456789\n";
    str << "abcdefghij\n";
    str << "klmnopqrst\n";
    str.close();

    std::string s;
    str.open("test.txt", std::ios::in);
    std::getline(str, s);
    EXPECT_EQ("0123456789", s);
    str.close();

    str.open("test.txt", std::ios::in);
    std::getline(str, s);
    EXPECT_EQ("0123456789", s);
    str.close();
}
