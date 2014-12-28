#include <gtest/gtest.h>

#include "lyxBinaryReader.h"
#include "lyxBinaryWriter.h"
#include "lyxBuffer.h"
#include <sstream>

using namespace lyx;

class BinaryReaderWriterTest: public testing::Test {
    public:
        void testWrite(BinaryWriter& writer) {
            writer << true;
            writer << false;
            writer << 'a';
            writer << (short) -100;
            writer << (unsigned short) 50000;
            writer << -123456;
            writer << (unsigned) 123456;
            writer << (long) -1234567890;
            writer << (unsigned long) 1234567890;
            writer << (int64_t) -1234567890;
            writer << (uint64_t) 1234567890;

            writer << (float) 1.5;
            writer << (double) -1.5;

            writer << "foo";
            writer << "";

            writer << std::string("bar");
            writer << std::string();

            writer.write7BitEncoded((uint32_t) 100);
            writer.write7BitEncoded((uint32_t) 1000);
            writer.write7BitEncoded((uint32_t) 10000);
            writer.write7BitEncoded((uint32_t) 100000);
            writer.write7BitEncoded((uint32_t) 1000000);

            writer.write7BitEncoded((uint64_t) 100);
            writer.write7BitEncoded((uint64_t) 1000);
            writer.write7BitEncoded((uint64_t) 10000);
            writer.write7BitEncoded((uint64_t) 100000);
            writer.write7BitEncoded((uint64_t) 1000000);

            std::vector<int> vec;
            vec.push_back(1);
            vec.push_back(2);
            vec.push_back(3);
            writer << vec;

            writer.writeRaw("RAW");
        }

        void testRead(BinaryReader& reader) {
            bool b;
            reader >> b;
            EXPECT_EQ (true, b);
            reader >> b;
            EXPECT_EQ (false, b);

            char c;
            reader >> c;
            EXPECT_EQ ('a', c);

            short shortv;
            reader >> shortv;
            EXPECT_EQ (-100, shortv);

            unsigned short ushortv;
            reader >> ushortv;
            EXPECT_EQ (50000, ushortv);

            int intv;
            reader >> intv;
            EXPECT_EQ (-123456, intv);

            unsigned int uintv;
            reader >> uintv;
            EXPECT_EQ (123456, uintv);

            long longv;
            reader >> longv;
            EXPECT_EQ (-1234567890, longv);

            unsigned long ulongv;
            reader >> ulongv;
            EXPECT_EQ (1234567890, ulongv);

            int64_t int64v;
            reader >> int64v;
            EXPECT_EQ (-1234567890, int64v);

            uint64_t uint64v;
            reader >> uint64v;
            EXPECT_EQ (1234567890, uint64v);

            float floatv;
            reader >> floatv;
            EXPECT_FLOAT_EQ (1.5, floatv);

            double doublev;
            reader >> doublev;
            EXPECT_DOUBLE_EQ (-1.5, doublev);

            std::string str;
            reader >> str;
            EXPECT_EQ("foo", str);
            reader >> str;
            EXPECT_EQ("", str);
            reader >> str;
            EXPECT_EQ("bar", str);
            reader >> str;
            EXPECT_EQ("", str);

            uint32_t uint32v;
            reader.read7BitEncoded(uint32v);
            EXPECT_EQ(100, uint32v);
            reader.read7BitEncoded(uint32v);
            EXPECT_EQ(1000, uint32v);
            reader.read7BitEncoded(uint32v);
            EXPECT_EQ(10000, uint32v);
            reader.read7BitEncoded(uint32v);
            EXPECT_EQ(100000, uint32v);
            reader.read7BitEncoded(uint32v);
            EXPECT_EQ(1000000, uint32v);

            reader.read7BitEncoded(uint64v);
            EXPECT_EQ(100, uint64v);
            reader.read7BitEncoded(uint64v);
            EXPECT_EQ(1000, uint64v);
            reader.read7BitEncoded(uint64v);
            EXPECT_EQ(10000, uint64v);
            reader.read7BitEncoded(uint64v);
            EXPECT_EQ(100000, uint64v);
            reader.read7BitEncoded(uint64v);
            EXPECT_EQ(1000000, uint64v);

            std::vector<int> vec;
            reader >> vec;
            EXPECT_EQ(3, vec.size());
            EXPECT_EQ(1, vec[0]);
            EXPECT_EQ(2, vec[1]);
            EXPECT_EQ(3, vec[2]);

            reader.readRaw(3, str);
            EXPECT_EQ("RAW", str);
        }
};

TEST_F(BinaryReaderWriterTest, testNative) {
    std::stringstream sstream;
    BinaryWriter writer(sstream);
    BinaryReader reader(sstream);
    testWrite(writer);
    testRead(reader);
}

TEST_F(BinaryReaderWriterTest, testWrappers) {
    bool b = false;
    char c = '0';
    int i = 0;
    Buffer<char> buf(2 * sizeof(bool) + sizeof(char) + 2 * sizeof(int));

    MemoryBinaryWriter writer(buf);
    writer << true;
    writer << false;
    writer << 'a';
    writer << 1;
    writer << -1;

    MemoryBinaryReader reader(writer.data());
    reader >> b; EXPECT_EQ(true, b);
    reader >> b; EXPECT_EQ(false, b);
    reader >> c; EXPECT_EQ('a', c);
    EXPECT_EQ(2 * sizeof(i), reader.available());
    reader >> i; EXPECT_EQ(1, i);
    EXPECT_EQ(sizeof(i), reader.available());
    reader >> i; EXPECT_EQ(-1, i);
    EXPECT_EQ(0, reader.available());

    reader.setExceptions(std::istream::eofbit);
    try {
        reader >> i;
        FAIL();
    }
    catch (std::exception&) {
    }
}

