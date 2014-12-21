#include <gtest/gtest.h>

#include "lyxDirectoryIterator.h"
#include "lyxFileStream.h"

using namespace lyx;

class DirectoryIteratorTest: public testing::Test {
    public:
        virtual void SetUp() {
            File d(path());
            if (d.exists()) d.remove(true);

            /*
             * Build Directory Tree like this:
             *
             * DirectoryIteratorTest
             *  |-- 1
             *  |-- 2
             *  |-- A
             *  `-- first
             *      |-- 1
             *      |-- 2
             *      |-- A
             *      `-- second
             *          |-- 1
             *          |-- 2
             *          |-- A
             */

            Path p = path();
            createSubdir(p);

            p.pushDirectory("first");
            createSubdir(p);

            p.pushDirectory("second");
            createSubdir(p);
        }

        virtual void TearDown() {
            try {
                File d(path());
                d.remove(true);
            }
            catch (...) {
            }
        }

        Path path() const {
            Path p(Path::current());
            p.pushDirectory("DirectoryIteratorTest");
            return p;
        }

        void createSubdir(Path& p) {
            File d(p);
            d.createDirectories();
            FileStream f1(p.toString() + "d");
            FileStream f2(p.toString() + "1");
            FileStream f3(p.toString() + "A");
            FileStream f4(p.toString() + "2");
            FileStream f5(p.toString() + "B");
            FileStream f6(p.toString() + "C");
        }
};


TEST_F(DirectoryIteratorTest, testDirectoryIterator) {
    Path p = path();
    DirectoryIterator dirIterator(p);
    DirectoryIterator end;
    std::vector<std::string> result;
    std::string file;

    while (dirIterator != end) {
        file = dirIterator->path();
        ++dirIterator;
        result.push_back(file);
    }

    EXPECT_EQ (7, (long) result.size());
}
