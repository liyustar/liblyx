#include <gtest/gtest.h>

#include "lyxPath.h"
#include "lyxException.h"

using namespace lyx;

TEST(PathTest, testParseUnix1)
{
    Path p;
    p.parse("", Path::PATH_UNIX);
    EXPECT_TRUE (p.isRelative());
    EXPECT_TRUE (!p.isAbsolute());
    EXPECT_TRUE (p.depth() == 0);
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_UNIX) == "");

    p.parse("/", Path::PATH_UNIX);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 0);
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_UNIX) == "/");

    p.parse("/usr", Path::PATH_UNIX);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 0);
    EXPECT_TRUE (!p.isDirectory());
    EXPECT_TRUE (p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_UNIX) == "/usr");

    p.parse("/usr/", Path::PATH_UNIX);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 1);
    EXPECT_TRUE (p[0] == "usr");
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_UNIX) == "/usr/");

    p.parse("usr/", Path::PATH_UNIX);
    EXPECT_TRUE (p.isRelative());
    EXPECT_TRUE (!p.isAbsolute());
    EXPECT_TRUE (p.depth() == 1);
    EXPECT_TRUE (p[0] == "usr");
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_UNIX) == "usr/");

    p.parse("usr", Path::PATH_UNIX);
    EXPECT_TRUE (p.isRelative());
    EXPECT_TRUE (!p.isAbsolute());
    EXPECT_TRUE (p.depth() == 0);
    EXPECT_TRUE (p[0] == "usr");
    EXPECT_TRUE (!p.isDirectory());
    EXPECT_TRUE (p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_UNIX) == "usr");

    p.parse("/usr/local", Path::PATH_UNIX);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 1);
    EXPECT_TRUE (p[0] == "usr");
    EXPECT_TRUE (p[1] == "local");
    EXPECT_TRUE (!p.isDirectory());
    EXPECT_TRUE (p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_UNIX) == "/usr/local");
}


TEST(PathTest, testParseUnix2)
{
    Path p;
    p.parse("/usr/local/", Path::PATH_UNIX);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 2);
    EXPECT_TRUE (p[0] == "usr");
    EXPECT_TRUE (p[1] == "local");
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_UNIX) == "/usr/local/");

    p.parse("usr/local/", Path::PATH_UNIX);
    EXPECT_TRUE (p.isRelative());
    EXPECT_TRUE (!p.isAbsolute());
    EXPECT_TRUE (p.depth() == 2);
    EXPECT_TRUE (p[0] == "usr");
    EXPECT_TRUE (p[1] == "local");
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_UNIX) == "usr/local/");

    p.parse("usr/local", Path::PATH_UNIX);
    EXPECT_TRUE (p.isRelative());
    EXPECT_TRUE (!p.isAbsolute());
    EXPECT_TRUE (p.depth() == 1);
    EXPECT_TRUE (p[0] == "usr");
    EXPECT_TRUE (p[1] == "local");
    EXPECT_TRUE (!p.isDirectory());
    EXPECT_TRUE (p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_UNIX) == "usr/local");

    p.parse("/usr/local/bin", Path::PATH_UNIX);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 2);
    EXPECT_TRUE (p[0] == "usr");
    EXPECT_TRUE (p[1] == "local");
    EXPECT_TRUE (p[2] == "bin");
    EXPECT_TRUE (!p.isDirectory());
    EXPECT_TRUE (p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_UNIX) == "/usr/local/bin");

    p.parse("/usr/local/bin/", Path::PATH_UNIX);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 3);
    EXPECT_TRUE (p[0] == "usr");
    EXPECT_TRUE (p[1] == "local");
    EXPECT_TRUE (p[2] == "bin");
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_UNIX) == "/usr/local/bin/");
}


TEST(PathTest, testParseUnix3)
{
    Path p;
    p.parse("//usr/local/bin/", Path::PATH_UNIX);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 3);
    EXPECT_TRUE (p[0] == "usr");
    EXPECT_TRUE (p[1] == "local");
    EXPECT_TRUE (p[2] == "bin");
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_UNIX) == "/usr/local/bin/");

    p.parse("/usr//local/bin/", Path::PATH_UNIX);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 3);
    EXPECT_TRUE (p[0] == "usr");
    EXPECT_TRUE (p[1] == "local");
    EXPECT_TRUE (p[2] == "bin");
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_UNIX) == "/usr/local/bin/");

    p.parse("/usr/local//bin/", Path::PATH_UNIX);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 3);
    EXPECT_TRUE (p[0] == "usr");
    EXPECT_TRUE (p[1] == "local");
    EXPECT_TRUE (p[2] == "bin");
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_UNIX) == "/usr/local/bin/");

    p.parse("/usr/local/bin//", Path::PATH_UNIX);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 3);
    EXPECT_TRUE (p[0] == "usr");
    EXPECT_TRUE (p[1] == "local");
    EXPECT_TRUE (p[2] == "bin");
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_UNIX) == "/usr/local/bin/");

    p.parse("/usr/local/./bin/", Path::PATH_UNIX);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 3);
    EXPECT_TRUE (p[0] == "usr");
    EXPECT_TRUE (p[1] == "local");
    EXPECT_TRUE (p[2] == "bin");
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_UNIX) == "/usr/local/bin/");

    p.parse("./usr/local/bin/", Path::PATH_UNIX);
    EXPECT_TRUE (p.isRelative());
    EXPECT_TRUE (!p.isAbsolute());
    EXPECT_TRUE (p.depth() == 3);
    EXPECT_TRUE (p[0] == "usr");
    EXPECT_TRUE (p[1] == "local");
    EXPECT_TRUE (p[2] == "bin");
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_UNIX) == "usr/local/bin/");

    p.parse("./usr/local/bin/./", Path::PATH_UNIX);
    EXPECT_TRUE (p.isRelative());
    EXPECT_TRUE (!p.isAbsolute());
    EXPECT_TRUE (p.depth() == 3);
    EXPECT_TRUE (p[0] == "usr");
    EXPECT_TRUE (p[1] == "local");
    EXPECT_TRUE (p[2] == "bin");
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_UNIX) == "usr/local/bin/");

    p.parse("./usr/local/bin/.", Path::PATH_UNIX);
    EXPECT_TRUE (p.isRelative());
    EXPECT_TRUE (!p.isAbsolute());
    EXPECT_TRUE (p.depth() == 3);
    EXPECT_TRUE (p[0] == "usr");
    EXPECT_TRUE (p[1] == "local");
    EXPECT_TRUE (p[2] == "bin");
    EXPECT_TRUE (!p.isDirectory());
    EXPECT_TRUE (p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_UNIX) == "usr/local/bin/.");
}


TEST(PathTest, testParseUnix4)
{
    Path p;
    p.parse("/usr/local/lib/../bin/", Path::PATH_UNIX);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 3);
    EXPECT_TRUE (p[0] == "usr");
    EXPECT_TRUE (p[1] == "local");
    EXPECT_TRUE (p[2] == "bin");
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_UNIX) == "/usr/local/bin/");

    p.parse("/usr/local/lib/../bin/", Path::PATH_UNIX);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 3);
    EXPECT_TRUE (p[0] == "usr");
    EXPECT_TRUE (p[1] == "local");
    EXPECT_TRUE (p[2] == "bin");
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_UNIX) == "/usr/local/bin/");

    p.parse("/usr/local/lib/../../", Path::PATH_UNIX);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 1);
    EXPECT_TRUE (p[0] == "usr");
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_UNIX) == "/usr/");

    p.parse("/usr/local/lib/..", Path::PATH_UNIX);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 3);
    EXPECT_TRUE (p[0] == "usr");
    EXPECT_TRUE (p[1] == "local");
    EXPECT_TRUE (p[2] == "lib");
    EXPECT_TRUE (!p.isDirectory());
    EXPECT_TRUE (p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_UNIX) == "/usr/local/lib/..");

    p.parse("../usr/local/lib/", Path::PATH_UNIX);
    EXPECT_TRUE (p.isRelative());
    EXPECT_TRUE (!p.isAbsolute());
    EXPECT_TRUE (p.depth() == 4);
    EXPECT_TRUE (p[0] == "..");
    EXPECT_TRUE (p[1] == "usr");
    EXPECT_TRUE (p[2] == "local");
    EXPECT_TRUE (p[3] == "lib");
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_UNIX) == "../usr/local/lib/");

    p.parse("/usr/../lib/", Path::PATH_UNIX);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 1);
    EXPECT_TRUE (p[0] == "lib");
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_UNIX) == "/lib/");

    p.parse("/usr/../../lib/", Path::PATH_UNIX);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 1);
    EXPECT_TRUE (p[0] == "lib");
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_UNIX) == "/lib/");

    p.parse("local/../../lib/", Path::PATH_UNIX);
    EXPECT_TRUE (p.isRelative());
    EXPECT_TRUE (!p.isAbsolute());
    EXPECT_TRUE (p.depth() == 2);
    EXPECT_TRUE (p[0] == "..");
    EXPECT_TRUE (p[1] == "lib");
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_UNIX) == "../lib/");

    p.parse("a/b/c/d", Path::PATH_UNIX);
    EXPECT_TRUE (p.isRelative());
    EXPECT_TRUE (!p.isAbsolute());
    EXPECT_TRUE (p.depth() == 3);
    EXPECT_TRUE (p[0] == "a");
    EXPECT_TRUE (p[1] == "b");
    EXPECT_TRUE (p[2] == "c");
    EXPECT_TRUE (!p.isDirectory());
    EXPECT_TRUE (p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_UNIX) == "a/b/c/d");
}


TEST(PathTest, testParseUnix5)
{
    Path p;
    p.parse("/c:/windows/system32/", Path::PATH_UNIX);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.getDevice() == "c");
    EXPECT_TRUE (p.depth() == 2);
    EXPECT_TRUE (p[0] == "windows");
    EXPECT_TRUE (p[1] == "system32");
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_UNIX) == "/c:/windows/system32/");	
}


TEST(PathTest, testParseWindows1)
{
    Path p;
    p.parse("", Path::PATH_WINDOWS);
    EXPECT_TRUE (p.isRelative());
    EXPECT_TRUE (!p.isAbsolute());
    EXPECT_TRUE (p.depth() == 0);
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "");

    p.parse("/", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 0);
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "\\");

    p.parse("\\", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 0);
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "\\");

    p.parse("/usr", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 0);
    EXPECT_TRUE (!p.isDirectory());
    EXPECT_TRUE (p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "\\usr");

    p.parse("\\usr", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 0);
    EXPECT_TRUE (!p.isDirectory());
    EXPECT_TRUE (p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "\\usr");

    p.parse("/usr/", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 1);
    EXPECT_TRUE (p[0] == "usr");
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "\\usr\\");

    p.parse("\\usr\\", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 1);
    EXPECT_TRUE (p[0] == "usr");
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "\\usr\\");
}


TEST(PathTest, testParseWindows2)
{
    Path p;
    p.parse("usr/", Path::PATH_WINDOWS);
    EXPECT_TRUE (p.isRelative());
    EXPECT_TRUE (!p.isAbsolute());
    EXPECT_TRUE (p.depth() == 1);
    EXPECT_TRUE (p[0] == "usr");
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "usr\\");

    p.parse("usr", Path::PATH_WINDOWS);
    EXPECT_TRUE (p.isRelative());
    EXPECT_TRUE (!p.isAbsolute());
    EXPECT_TRUE (p.depth() == 0);
    EXPECT_TRUE (p[0] == "usr");
    EXPECT_TRUE (!p.isDirectory());
    EXPECT_TRUE (p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "usr");

    p.parse("usr\\", Path::PATH_WINDOWS);
    EXPECT_TRUE (p.isRelative());
    EXPECT_TRUE (!p.isAbsolute());
    EXPECT_TRUE (p.depth() == 1);
    EXPECT_TRUE (p[0] == "usr");
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "usr\\");

    p.parse("/usr/local", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 1);
    EXPECT_TRUE (p[0] == "usr");
    EXPECT_TRUE (p[1] == "local");
    EXPECT_TRUE (!p.isDirectory());
    EXPECT_TRUE (p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "\\usr\\local");

    p.parse("\\usr\\local", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 1);
    EXPECT_TRUE (p[0] == "usr");
    EXPECT_TRUE (p[1] == "local");
    EXPECT_TRUE (!p.isDirectory());
    EXPECT_TRUE (p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "\\usr\\local");

    p.parse("/usr/local/", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 2);
    EXPECT_TRUE (p[0] == "usr");
    EXPECT_TRUE (p[1] == "local");
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "\\usr\\local\\");

    p.parse("usr/local/", Path::PATH_WINDOWS);
    EXPECT_TRUE (p.isRelative());
    EXPECT_TRUE (!p.isAbsolute());
    EXPECT_TRUE (p.depth() == 2);
    EXPECT_TRUE (p[0] == "usr");
    EXPECT_TRUE (p[1] == "local");
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "usr\\local\\");

    p.parse("usr/local", Path::PATH_WINDOWS);
    EXPECT_TRUE (p.isRelative());
    EXPECT_TRUE (!p.isAbsolute());
    EXPECT_TRUE (p.depth() == 1);
    EXPECT_TRUE (p[0] == "usr");
    EXPECT_TRUE (p[1] == "local");
    EXPECT_TRUE (!p.isDirectory());
    EXPECT_TRUE (p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "usr\\local");

    p.parse("/usr/local/bin", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 2);
    EXPECT_TRUE (p[0] == "usr");
    EXPECT_TRUE (p[1] == "local");
    EXPECT_TRUE (p[2] == "bin");
    EXPECT_TRUE (!p.isDirectory());
    EXPECT_TRUE (p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "\\usr\\local\\bin");

    p.parse("/usr/local/bin/", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 3);
    EXPECT_TRUE (p[0] == "usr");
    EXPECT_TRUE (p[1] == "local");
    EXPECT_TRUE (p[2] == "bin");
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "\\usr\\local\\bin\\");

    p.parse("/usr//local/bin/", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 3);
    EXPECT_TRUE (p[0] == "usr");
    EXPECT_TRUE (p[1] == "local");
    EXPECT_TRUE (p[2] == "bin");
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "\\usr\\local\\bin\\");

    p.parse("/usr/local//bin/", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 3);
    EXPECT_TRUE (p[0] == "usr");
    EXPECT_TRUE (p[1] == "local");
    EXPECT_TRUE (p[2] == "bin");
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "\\usr\\local\\bin\\");

    p.parse("/usr/local/bin//", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 3);
    EXPECT_TRUE (p[0] == "usr");
    EXPECT_TRUE (p[1] == "local");
    EXPECT_TRUE (p[2] == "bin");
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "\\usr\\local\\bin\\");
}


TEST(PathTest, testParseWindows3)
{
    Path p;
    p.parse("/usr/local/./bin/", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 3);
    EXPECT_TRUE (p[0] == "usr");
    EXPECT_TRUE (p[1] == "local");
    EXPECT_TRUE (p[2] == "bin");
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "\\usr\\local\\bin\\");

    p.parse("./usr/local/bin/", Path::PATH_WINDOWS);
    EXPECT_TRUE (p.isRelative());
    EXPECT_TRUE (!p.isAbsolute());
    EXPECT_TRUE (p.depth() == 3);
    EXPECT_TRUE (p[0] == "usr");
    EXPECT_TRUE (p[1] == "local");
    EXPECT_TRUE (p[2] == "bin");
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "usr\\local\\bin\\");

    p.parse("./usr/local/bin/./", Path::PATH_WINDOWS);
    EXPECT_TRUE (p.isRelative());
    EXPECT_TRUE (!p.isAbsolute());
    EXPECT_TRUE (p.depth() == 3);
    EXPECT_TRUE (p[0] == "usr");
    EXPECT_TRUE (p[1] == "local");
    EXPECT_TRUE (p[2] == "bin");
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "usr\\local\\bin\\");

    p.parse("./usr/local/bin/.", Path::PATH_WINDOWS);
    EXPECT_TRUE (p.isRelative());
    EXPECT_TRUE (!p.isAbsolute());
    EXPECT_TRUE (p.depth() == 3);
    EXPECT_TRUE (p[0] == "usr");
    EXPECT_TRUE (p[1] == "local");
    EXPECT_TRUE (p[2] == "bin");
    EXPECT_TRUE (!p.isDirectory());
    EXPECT_TRUE (p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "usr\\local\\bin\\.");

    p.parse("/usr/local/lib/../bin/", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 3);
    EXPECT_TRUE (p[0] == "usr");
    EXPECT_TRUE (p[1] == "local");
    EXPECT_TRUE (p[2] == "bin");
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "\\usr\\local\\bin\\");

    p.parse("/usr/local/lib/../bin/", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 3);
    EXPECT_TRUE (p[0] == "usr");
    EXPECT_TRUE (p[1] == "local");
    EXPECT_TRUE (p[2] == "bin");
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "\\usr\\local\\bin\\");

    p.parse("\\usr\\local\\lib\\..\\bin\\", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 3);
    EXPECT_TRUE (p[0] == "usr");
    EXPECT_TRUE (p[1] == "local");
    EXPECT_TRUE (p[2] == "bin");
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "\\usr\\local\\bin\\");

    p.parse("/usr/local/lib/../../", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 1);
    EXPECT_TRUE (p[0] == "usr");
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "\\usr\\");

    p.parse("/usr/local/lib/..", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 3);
    EXPECT_TRUE (p[0] == "usr");
    EXPECT_TRUE (p[1] == "local");
    EXPECT_TRUE (p[2] == "lib");
    EXPECT_TRUE (!p.isDirectory());
    EXPECT_TRUE (p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "\\usr\\local\\lib\\..");

    p.parse("../usr/local/lib/", Path::PATH_WINDOWS);
    EXPECT_TRUE (p.isRelative());
    EXPECT_TRUE (!p.isAbsolute());
    EXPECT_TRUE (p.depth() == 4);
    EXPECT_TRUE (p[0] == "..");
    EXPECT_TRUE (p[1] == "usr");
    EXPECT_TRUE (p[2] == "local");
    EXPECT_TRUE (p[3] == "lib");
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "..\\usr\\local\\lib\\");

    p.parse("/usr/../lib/", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 1);
    EXPECT_TRUE (p[0] == "lib");
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "\\lib\\");

    p.parse("/usr/../../lib/", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 1);
    EXPECT_TRUE (p[0] == "lib");
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "\\lib\\");

    p.parse("local/../../lib/", Path::PATH_WINDOWS);
    EXPECT_TRUE (p.isRelative());
    EXPECT_TRUE (!p.isAbsolute());
    EXPECT_TRUE (p.depth() == 2);
    EXPECT_TRUE (p[0] == "..");
    EXPECT_TRUE (p[1] == "lib");
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "..\\lib\\");
}


TEST(PathTest, testParseWindows4)
{
    Path p;
    p.parse("\\\\server\\files", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 1);
    EXPECT_TRUE (p[0] == "files");
    EXPECT_TRUE (p.getNode() == "server");
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "\\\\server\\files\\");

    p.parse("\\\\server\\files\\", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 1);
    EXPECT_TRUE (p[0] == "files");
    EXPECT_TRUE (p.getNode() == "server");
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "\\\\server\\files\\");

    p.parse("\\\\server\\files\\file", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 1);
    EXPECT_TRUE (p[0] == "files");
    EXPECT_TRUE (p.getNode() == "server");
    EXPECT_TRUE (!p.isDirectory());
    EXPECT_TRUE (p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "\\\\server\\files\\file");

    p.parse("\\\\server\\files\\dir\\file", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 2);
    EXPECT_TRUE (p[0] == "files");
    EXPECT_TRUE (p[1] == "dir");
    EXPECT_TRUE (p.getNode() == "server");
    EXPECT_TRUE (!p.isDirectory());
    EXPECT_TRUE (p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "\\\\server\\files\\dir\\file");

    p.parse("\\\\server\\files\\dir\\file", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 2);
    EXPECT_TRUE (p[0] == "files");
    EXPECT_TRUE (p[1] == "dir");
    EXPECT_TRUE (p.getNode() == "server");
    EXPECT_TRUE (!p.isDirectory());
    EXPECT_TRUE (p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "\\\\server\\files\\dir\\file");	

    p.parse("\\\\server", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 0);
    EXPECT_TRUE (p.getNode() == "server");
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "\\\\server\\");

    p.parse("c:\\", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 0);
    EXPECT_TRUE (p.getDevice() == "c");
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "c:\\");	

    p.parse("c:\\WinNT", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 0);
    EXPECT_TRUE (p.getDevice() == "c");
    EXPECT_TRUE (!p.isDirectory());
    EXPECT_TRUE (p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "c:\\WinNT");	

    p.parse("c:\\WinNT\\", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.depth() == 1);
    EXPECT_TRUE (p[0] == "WinNT");
    EXPECT_TRUE (p.getDevice() == "c");
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "c:\\WinNT\\");

    try
    {
        p.parse("~:\\", Path::PATH_WINDOWS);
        // fail("bad path - must throw exception");
        FAIL();
    }
    catch (PathSyntaxException&)
    {
    }

    try
    {
        p.parse("c:file.txt", Path::PATH_WINDOWS);
        // fail("bad path - must throw exception");
        FAIL();
    }
    catch (PathSyntaxException&)
    {
    }

    p.parse("a\\b\\c\\d", Path::PATH_WINDOWS);
    EXPECT_TRUE (p.isRelative());
    EXPECT_TRUE (!p.isAbsolute());
    EXPECT_TRUE (p.depth() == 3);
    EXPECT_TRUE (p[0] == "a");
    EXPECT_TRUE (p[1] == "b");
    EXPECT_TRUE (p[2] == "c");
    EXPECT_TRUE (!p.isDirectory());
    EXPECT_TRUE (p.isFile());
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "a\\b\\c\\d");
}
TEST(PathTest, testTryParse)
{
    Path p;
    EXPECT_TRUE (p.tryParse("/etc/passwd"));
    EXPECT_TRUE (p.toString() == "/etc/passwd");

    EXPECT_TRUE (p.tryParse("c:\\windows\\system", Path::PATH_WINDOWS));
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "c:\\windows\\system");
    EXPECT_TRUE (!p.tryParse("c:foo.bar", Path::PATH_WINDOWS));
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "c:\\windows\\system");
}


TEST(PathTest, testStatics)
{
    std::string s = Path::current();
    EXPECT_TRUE (!s.empty());
    Path p(s);
    EXPECT_TRUE (p.isDirectory() && p.isAbsolute());

    s = Path::home();
    EXPECT_TRUE (!s.empty());
    p = s;
    EXPECT_TRUE (p.isDirectory() && p.isAbsolute());

    s = Path::temp();
    EXPECT_TRUE (!s.empty());
    p = s;
    EXPECT_TRUE (p.isDirectory() && p.isAbsolute());

    s = Path::null();
    EXPECT_TRUE (!s.empty());
    p = s;
}


TEST(PathTest, testBaseNameExt)
{
    Path p("foo.bar");
    EXPECT_TRUE (p.getFileName() == "foo.bar");
    EXPECT_TRUE (p.getBaseName() == "foo");
    EXPECT_TRUE (p.getExtension() == "bar");

    p.setBaseName("readme");
    EXPECT_TRUE (p.getFileName() == "readme.bar");
    EXPECT_TRUE (p.getBaseName() == "readme");
    EXPECT_TRUE (p.getExtension() == "bar");

    p.setExtension("txt");
    EXPECT_TRUE (p.getFileName() == "readme.txt");
    EXPECT_TRUE (p.getBaseName() == "readme");
    EXPECT_TRUE (p.getExtension() == "txt");

    p.setExtension("html");
    EXPECT_TRUE (p.getFileName() == "readme.html");
    EXPECT_TRUE (p.getBaseName() == "readme");
    EXPECT_TRUE (p.getExtension() == "html");

    p.setBaseName("index");
    EXPECT_TRUE (p.getFileName() == "index.html");
    EXPECT_TRUE (p.getBaseName() == "index");
    EXPECT_TRUE (p.getExtension() == "html");
}


TEST(PathTest, testAbsolute)
{
    Path base("C:\\Program Files\\", Path::PATH_WINDOWS);
    Path rel("lyx");
    Path abs = rel.absolute(base);
    EXPECT_TRUE (abs.toString(Path::PATH_WINDOWS) == "C:\\Program Files\\lyx");

    base.parse("/usr/local", Path::PATH_UNIX);
    rel.parse("lyx/include", Path::PATH_UNIX);
    abs = rel.absolute(base);
    EXPECT_TRUE (abs.toString(Path::PATH_UNIX) == "/usr/local/lyx/include");

    base.parse("/usr/local/bin", Path::PATH_UNIX);
    rel.parse("../lyx/include", Path::PATH_UNIX);
    abs = rel.absolute(base);
    EXPECT_TRUE (abs.toString(Path::PATH_UNIX) == "/usr/local/lyx/include");
}

/*
TEST(PathTest, testRobustness)
{
    lyx::Random r;
    for (int i = 0; i < 256; ++i)
    {
        int len = r.next(1024);
        std::string s;
        for (int i = 0; i < len; ++i) s += r.nextChar();
        try
        {
            Path p(s, Path::PATH_WINDOWS);
        }
        catch (PathSyntaxException&)
        {
        }
        try
        {
            Path p(s, Path::PATH_UNIX);
        }
        catch (PathSyntaxException&)
        {
        }
    }
}
*/

TEST(PathTest, testParent)
{
    Path p("/usr/local/include", Path::PATH_UNIX);
    p.makeParent();
    EXPECT_TRUE (p.toString(Path::PATH_UNIX) == "/usr/local/");
    p.makeParent();
    EXPECT_TRUE (p.toString(Path::PATH_UNIX) == "/usr/");
    p.makeParent();
    EXPECT_TRUE (p.toString(Path::PATH_UNIX) == "/");
    p.makeParent();
    EXPECT_TRUE (p.toString(Path::PATH_UNIX) == "/");
}


TEST(PathTest, testForDirectory)
{
    Path p = Path::forDirectory("/usr/local/include", Path::PATH_UNIX);
    EXPECT_TRUE (p.toString(Path::PATH_UNIX) == "/usr/local/include/");

    p = Path::forDirectory("/usr/local/include/", Path::PATH_UNIX);
    EXPECT_TRUE (p.toString(Path::PATH_UNIX) == "/usr/local/include/");
}


TEST(PathTest, testExpand)
{
    std::string s = Path::expand("~/.bashrc");
    EXPECT_TRUE (s == Path::expand("$HOME/.bashrc"));
    /*
    EXPECT_TRUE (s == Environment::get("HOME") + "/.bashrc" || 
            s == Environment::get("HOME") + "//.bashrc");
            */
    Path p(s);
    s = Path::expand("$HOME/.bashrc");
    EXPECT_TRUE (s == Path::expand("~/.bashrc"));
    s = Path::expand("${HOME}/.bashrc");
    EXPECT_TRUE (s == Path::expand("~/.bashrc"));
}


TEST(PathTest, testListRoots)
{
    std::vector<std::string> devs;
    Path::listRoots(devs);
    EXPECT_TRUE (devs.size() > 0);
    for (std::vector<std::string>::iterator it = devs.begin(); it != devs.end(); ++it)
    {
        std::cout << *it << std::endl;
    }
}

/*
TEST(PathTest, testFind)
{
    Path p;
    bool found = Path::find(Environment::get("PATH"), "ls", p);
    bool notfound = Path::find(Environment::get("PATH"), "xxxyyy123", p);

    EXPECT_TRUE (found);
    EXPECT_TRUE (!notfound);

    std::string fn = p.toString();
    EXPECT_TRUE (fn.size() > 0);
}
*/

TEST(PathTest, testSwap)
{
    Path p1("c:\\temp\\foo.bar");
    Path p2("\\\\server\\files\\foo.bar");
    p1.swap(p2);
    EXPECT_TRUE (p1.toString() == "\\\\server\\files\\foo.bar");
    EXPECT_TRUE (p2.toString() == "c:\\temp\\foo.bar");
}


TEST(PathTest, testResolve)
{
    Path p("c:\\foo\\", Path::PATH_WINDOWS);
    p.resolve("test.dat");
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "c:\\foo\\test.dat");

    p.assign("c:\\foo\\", Path::PATH_WINDOWS);
    p.resolve(Path("d:\\bar.txt", Path::PATH_WINDOWS));
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "d:\\bar.txt");

    p.assign("c:\\foo\\bar.txt", Path::PATH_WINDOWS);
    p.resolve("foo.txt");
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "c:\\foo\\foo.txt");

    p.assign("c:\\foo\\bar\\", Path::PATH_WINDOWS);
    p.resolve(Path("..\\baz\\test.dat", Path::PATH_WINDOWS));
    EXPECT_TRUE (p.toString(Path::PATH_WINDOWS) == "c:\\foo\\baz\\test.dat");
}


TEST(PathTest, testPushPop)
{
    Path p;
    p.pushDirectory("a");
    p.pushDirectory("b");
    p.pushDirectory("c");
    EXPECT_TRUE (p.toString(Path::PATH_UNIX) == "a/b/c/");

    p.popDirectory();
    EXPECT_TRUE (p.toString(Path::PATH_UNIX) == "a/b/");

    p.popFrontDirectory();
    EXPECT_TRUE (p.toString(Path::PATH_UNIX) == "b/");
}
