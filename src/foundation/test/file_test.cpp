#include <gtest/gtest.h>

#include "lyxFile.h"
#include "lyxTemporaryFile.h"
#include "lyxException.h"
#include <fstream>

using namespace lyx;

class FileTest: public testing::Test {
    public:
        virtual void SetUp() {
            File f("testfile.dat");
            try {
                f.remove();
            }
            catch (...) {
            }
        }

        virtual void TearDown() {
            File f("testfile.dat");
            try {
                f.remove();
            }
            catch (...) {
            }
        }
};

TEST(FileTest, testFileAttributes1) {
    File f("testfile.dat");
    EXPECT_TRUE (!f.exists());

    try {
        bool flag = f.canRead();
        FAIL(); // file does not exist - must throw exception
    }
    catch (Exception&) {
    }

    try {
        bool flag = f.canWrite();
        FAIL(); // file does not exist - must throw exception
    }
    catch (Exception&) {
    }

    try {
        bool flag = f.isFile();
        FAIL(); // file does not exist - must throw exception
    }
    catch (Exception&) {
    }

    try {
        bool flag = f.isDirectory();
        FAIL(); // file does not exist - must throw exception
    }
    catch (Exception&) {
    }

    try {
        Timestamp ts = f.created();
        FAIL(); // file does not exist - must throw exception
    }
    catch (Exception&) {
    }

    try {
        Timestamp ts = f.getLastModified();
        FAIL(); // file does not exist - must throw exception
    }
    catch (Exception&) {
    }

    try {
        Timestamp ts;
        f.setLastModified(ts);
        FAIL(); // file does not exist - must throw exception
    }
    catch (Exception&) {
    }

    try {
        File::FileSize fs = f.getSize();
        FAIL(); // file does not exist - must throw exception
    }
    catch (Exception&) {
    }

    try {
        f.setSize(0);
        FAIL(); // file does not exist - must throw exception
    }
    catch (Exception&) {
    }

    try {
        f.setWriteable();
        FAIL(); // file does not exist - must throw exception
    }
    catch (Exception&) {
    }

    try {
        f.setReadOnly();
        FAIL(); // file does not exist - must throw exception
    }
    catch (Exception&) {
    }

    try {
        f.copyTo("copy.dat");
        FAIL(); // file does not exist - must throw exception
    }
    catch (Exception&) {
    }

    try {
        f.moveTo("copy.dat");
        FAIL(); // file does not exist - must throw exception
    }
    catch (Exception&) {
    }

    try {
        f.renameTo("copy.dat");
        FAIL(); // file does not exist - must throw exception
    }
    catch (Exception&) {
    }

    try {
        f.remove();
        FAIL(); // file does not exist - must throw exception
    }
    catch (Exception&) {
    }
}

TEST(FileTest, testCreateFile) {
    File f("testfile.dat");
    bool created = f.createFile();
    EXPECT_TRUE (created);
    EXPECT_TRUE (!f.isHidden());
    created = f.createFile();
    EXPECT_TRUE (!created);
}

TEST(FileTest, testFileAttributes2) {
    TemporaryFile f;
    bool created = f.createFile();
    Timestamp ts;
    EXPECT_TRUE (created);

    EXPECT_TRUE (f.exists());
    EXPECT_TRUE (f.canRead());
    EXPECT_TRUE (f.canWrite());
    EXPECT_TRUE (f.isFile());
    EXPECT_TRUE (!f.isDirectory());
    Timestamp tsc = f.created();
    Timestamp tsm = f.getLastModified();
    EXPECT_TRUE (tsc - ts >= -2000000 && tsc - ts <= 2000000);
    EXPECT_TRUE (tsm - ts >= -2000000 && tsm - ts <= 2000000);

    f.setWriteable(false);
    EXPECT_TRUE (!f.canWrite());
    EXPECT_TRUE (f.canRead());

    f.setReadOnly(false);
    EXPECT_TRUE (f.canWrite());
    EXPECT_TRUE (f.canRead());

    ts = Timestamp::fromEpochTime(1000000);
    f.setLastModified(ts);
    EXPECT_EQ (ts, f.getLastModified());
}

TEST(FileTest, testFileAttributes3) {
    File f("/dev/console");

    EXPECT_TRUE (f.isDevice());
    EXPECT_TRUE (!f.isFile());
    EXPECT_TRUE (!f.isDirectory());
}

TEST(FileTest, testCompare) {
    File f1("abc.txt");
    File f2("def.txt");
    File f3("abc.txt");

    EXPECT_TRUE (f1 == f3);
    EXPECT_TRUE (!(f1 == f2));
    EXPECT_TRUE (f1 != f2);
    EXPECT_TRUE (!(f1 != f3));
    EXPECT_TRUE (!(f1 == f2));
    EXPECT_TRUE (f1 < f2);
    EXPECT_TRUE (f1 <= f2);
    EXPECT_TRUE (!(f2 < f1));
    EXPECT_TRUE (!(f2 <= f1));
    EXPECT_TRUE (f2 > f1);
    EXPECT_TRUE (f2 >= f1);
    EXPECT_TRUE (!(f1 > f2));
    EXPECT_TRUE (!(f1 >= f2));

    EXPECT_TRUE (f1 <= f3);
    EXPECT_TRUE (f1 >= f3);
}

TEST(FileTest, testRootDir) {
    File f1("/");
    EXPECT_TRUE (f1.exists());
}

TEST(FileTest, testSwap) {
    File f1("abc.txt");
    File f2("def.txt");
    f1.swap(f2);
    EXPECT_EQ ("def.txt", f1.path());
    EXPECT_EQ ("abc.txt", f2.path());
}

TEST(FileTest, testSize) {
    std::ofstream ostr("testfile.dat");
    ostr << "Hello, world!" << std::endl;
    ostr.close();
    File f("testfile.dat");
    EXPECT_TRUE (f.getSize() > 0);
    EXPECT_EQ (14, f.getSize());
    f.setSize(0);
    EXPECT_EQ (0, f.getSize());
}

TEST(FileTest, testDirectory) {
    File d("testdir");
    try {
        d.remove(true);
    }
    catch (...) {
    }

    TemporaryFile::registerForDeletion("testdir");

    bool created = d.createDirectory();
    EXPECT_TRUE (created);
    EXPECT_TRUE (d.isDirectory());
    EXPECT_TRUE (!d.isFile());
    std::vector<std::string> files;
    d.list(files);
    EXPECT_TRUE (files.empty());

    File f = Path("testdir/file1", Path::PATH_UNIX);
    f.createFile();
    f = Path("testdir/file2", Path::PATH_UNIX);
    f.createFile();
    f = Path("testdir/file3", Path::PATH_UNIX);
    f.createFile();

    d.list(files);
    EXPECT_EQ (3, files.size());

    std::set<std::string> fs;
    fs.insert(files.begin(), files.end());
    EXPECT_TRUE (fs.find("file1") != fs.end());
    EXPECT_TRUE (fs.find("file2") != fs.end());
    EXPECT_TRUE (fs.find("file3") != fs.end());

    File dd(Path("testdir/testdir2/testdir3", Path::PATH_UNIX));
    dd.createDirectories();
    EXPECT_TRUE (dd.exists());
    EXPECT_TRUE (dd.isDirectory());

    File ddd(Path("testdir/testdirB/testdirC/testdirD", Path::PATH_UNIX));
    ddd.createDirectories();
    EXPECT_TRUE (ddd.exists());
    EXPECT_TRUE (ddd.isDirectory());

    d.remove(true);
}

TEST(FileTest, testCopy) {
    std::ofstream ostr("testfile.dat");
    ostr << "Hello, world!" << std::endl;
    ostr.close();

    File f1("testfile.dat");
    TemporaryFile f2;
    f1.copyTo(f2.path());
    EXPECT_TRUE (f2.exists());
    EXPECT_TRUE (f1.getSize() == f2.getSize());
}

TEST(FileTest, testMove) {
    std::ofstream ostr("testfile.dat");
    ostr << "Hello, world!" << std::endl;
    ostr.close();

    File f1("testfile.dat");
    File::FileSize sz = f1.getSize();
    TemporaryFile f2;
    f1.moveTo(f2.path());
    EXPECT_TRUE (f2.exists());
    EXPECT_EQ (sz, f2.getSize());
    EXPECT_TRUE (f1.exists());
    EXPECT_TRUE (f1 == f2);
}

TEST(FileTest, testCopyDirectory) {
    Path pd1("testdir");
    File fd1(pd1);
    try {
        fd1.remove(true);
    }
    catch (...) {
    }

    fd1.createDirectories();
    Path pd2(pd1, "subdir");
    File fd2(pd2);
    fd2.createDirectories();
    Path pf1(pd1, "testfile1.dat");
    std::ofstream ostr1(pf1.toString().c_str());
    ostr1 << "Hello, world!" << std::endl;
    ostr1.close();
    Path pf2(pd1, "testfile2.dat");
    std::ofstream ostr2(pf2.toString().c_str());
    ostr2 << "Hello, world!" << std::endl;
    ostr2.close();
    Path pf3(pd2, "testfile3.dat");
    std::ofstream ostr3(pf3.toString().c_str());
    ostr3 << "Hello, world!" << std::endl;
    ostr3.close();

    File fd3("testdir2");

    try {
        fd3.remove(true);
    }
    catch (...) {
    }

    fd1.copyTo("testdir2");

    Path pd1t("testdir2");
    File fd1t(pd1t);
    EXPECT_TRUE (fd1t.exists());
    EXPECT_TRUE (fd1t.isDirectory());

    Path pd2t(pd1t, "subdir");
    File fd2t(pd2t);
    EXPECT_TRUE (fd2t.exists());
    EXPECT_TRUE (fd2t.isDirectory());

    Path pf1t(pd1t, "testfile1.dat");
    File ff1t(pf1t);
    EXPECT_TRUE (ff1t.exists());
    EXPECT_TRUE (ff1t.isFile());

    Path pf2t(pd1t, "testfile2.dat");
    File ff2t(pf2t);
    EXPECT_TRUE (ff2t.exists());
    EXPECT_TRUE (ff2t.isFile());

    Path pf3t(pd2t, "testfile3.dat");
    File ff3t(pf3t);
    EXPECT_TRUE (ff3t.exists());
    EXPECT_TRUE (ff3t.isFile());

    fd1.remove(true);
    fd3.remove(true);
}

TEST(FileTest, testRename) {
    std::ofstream ostr("testfile.dat");
    ostr << "Hello, world!" << std::endl;
    ostr.close();

    File f1("testfile.dat");
    File f2("testfile2.dat");
    f1.renameTo(f2.path());

    EXPECT_TRUE (f1.exists());
    EXPECT_TRUE (f2.exists());
    EXPECT_TRUE (f1 == f2);

    f2.remove();
}
