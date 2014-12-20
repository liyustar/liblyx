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
    EXPECT_EQ (0, p.depth());
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("", p.toString(Path::PATH_UNIX));

    p.parse("/", Path::PATH_UNIX);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (0, p.depth());
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("/", p.toString(Path::PATH_UNIX));

    p.parse("/usr", Path::PATH_UNIX);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (0, p.depth());
    EXPECT_TRUE (!p.isDirectory());
    EXPECT_TRUE (p.isFile());
    EXPECT_EQ ("/usr", p.toString(Path::PATH_UNIX));

    p.parse("/usr/", Path::PATH_UNIX);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (1, p.depth());
    EXPECT_EQ ("usr", p[0]);
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("/usr/", p.toString(Path::PATH_UNIX));

    p.parse("usr/", Path::PATH_UNIX);
    EXPECT_TRUE (p.isRelative());
    EXPECT_TRUE (!p.isAbsolute());
    EXPECT_EQ (1, p.depth());
    EXPECT_EQ ("usr", p[0]);
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("usr/", p.toString(Path::PATH_UNIX));

    p.parse("usr", Path::PATH_UNIX);
    EXPECT_TRUE (p.isRelative());
    EXPECT_TRUE (!p.isAbsolute());
    EXPECT_EQ (0, p.depth());
    EXPECT_EQ ("usr", p[0]);
    EXPECT_TRUE (!p.isDirectory());
    EXPECT_TRUE (p.isFile());
    EXPECT_EQ ("usr", p.toString(Path::PATH_UNIX));

    p.parse("/usr/local", Path::PATH_UNIX);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (1, p.depth());
    EXPECT_EQ ("usr", p[0]);
    EXPECT_EQ ("local", p[1]);
    EXPECT_TRUE (!p.isDirectory());
    EXPECT_TRUE (p.isFile());
    EXPECT_EQ ("/usr/local", p.toString(Path::PATH_UNIX));
}


TEST(PathTest, testParseUnix2)
{
    Path p;
    p.parse("/usr/local/", Path::PATH_UNIX);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (2, p.depth());
    EXPECT_EQ ("usr", p[0]);
    EXPECT_EQ ("local", p[1]);
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("/usr/local/", p.toString(Path::PATH_UNIX));

    p.parse("usr/local/", Path::PATH_UNIX);
    EXPECT_TRUE (p.isRelative());
    EXPECT_TRUE (!p.isAbsolute());
    EXPECT_EQ (2, p.depth());
    EXPECT_EQ ("usr", p[0]);
    EXPECT_EQ ("local", p[1]);
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("usr/local/", p.toString(Path::PATH_UNIX));

    p.parse("usr/local", Path::PATH_UNIX);
    EXPECT_TRUE (p.isRelative());
    EXPECT_TRUE (!p.isAbsolute());
    EXPECT_EQ (1, p.depth());
    EXPECT_EQ ("usr", p[0]);
    EXPECT_EQ ("local", p[1]);
    EXPECT_TRUE (!p.isDirectory());
    EXPECT_TRUE (p.isFile());
    EXPECT_EQ ("usr/local", p.toString(Path::PATH_UNIX));

    p.parse("/usr/local/bin", Path::PATH_UNIX);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (2, p.depth());
    EXPECT_EQ ("usr", p[0]);
    EXPECT_EQ ("local", p[1]);
    EXPECT_EQ ("bin", p[2]);
    EXPECT_TRUE (!p.isDirectory());
    EXPECT_TRUE (p.isFile());
    EXPECT_EQ ("/usr/local/bin", p.toString(Path::PATH_UNIX));

    p.parse("/usr/local/bin/", Path::PATH_UNIX);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (3, p.depth());
    EXPECT_EQ ("usr", p[0]);
    EXPECT_EQ ("local", p[1]);
    EXPECT_EQ ("bin", p[2]);
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("/usr/local/bin/", p.toString(Path::PATH_UNIX));
}


TEST(PathTest, testParseUnix3)
{
    Path p;
    p.parse("//usr/local/bin/", Path::PATH_UNIX);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (3, p.depth());
    EXPECT_EQ ("usr", p[0]);
    EXPECT_EQ ("local", p[1]);
    EXPECT_EQ ("bin", p[2]);
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("/usr/local/bin/", p.toString(Path::PATH_UNIX));

    p.parse("/usr//local/bin/", Path::PATH_UNIX);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (3, p.depth());
    EXPECT_EQ ("usr", p[0]);
    EXPECT_EQ ("local", p[1]);
    EXPECT_EQ ("bin", p[2]);
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("/usr/local/bin/", p.toString(Path::PATH_UNIX));

    p.parse("/usr/local//bin/", Path::PATH_UNIX);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (3, p.depth());
    EXPECT_EQ ("usr", p[0]);
    EXPECT_EQ ("local", p[1]);
    EXPECT_EQ ("bin", p[2]);
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("/usr/local/bin/", p.toString(Path::PATH_UNIX));

    p.parse("/usr/local/bin//", Path::PATH_UNIX);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (3, p.depth());
    EXPECT_EQ ("usr", p[0]);
    EXPECT_EQ ("local", p[1]);
    EXPECT_EQ ("bin", p[2]);
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("/usr/local/bin/", p.toString(Path::PATH_UNIX));

    p.parse("/usr/local/./bin/", Path::PATH_UNIX);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (3, p.depth());
    EXPECT_EQ ("usr", p[0]);
    EXPECT_EQ ("local", p[1]);
    EXPECT_EQ ("bin", p[2]);
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("/usr/local/bin/", p.toString(Path::PATH_UNIX));

    p.parse("./usr/local/bin/", Path::PATH_UNIX);
    EXPECT_TRUE (p.isRelative());
    EXPECT_TRUE (!p.isAbsolute());
    EXPECT_EQ (3, p.depth());
    EXPECT_EQ ("usr", p[0]);
    EXPECT_EQ ("local", p[1]);
    EXPECT_EQ ("bin", p[2]);
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("usr/local/bin/", p.toString(Path::PATH_UNIX));

    p.parse("./usr/local/bin/./", Path::PATH_UNIX);
    EXPECT_TRUE (p.isRelative());
    EXPECT_TRUE (!p.isAbsolute());
    EXPECT_EQ (3, p.depth());
    EXPECT_EQ ("usr", p[0]);
    EXPECT_EQ ("local", p[1]);
    EXPECT_EQ ("bin", p[2]);
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("usr/local/bin/", p.toString(Path::PATH_UNIX));

    p.parse("./usr/local/bin/.", Path::PATH_UNIX);
    EXPECT_TRUE (p.isRelative());
    EXPECT_TRUE (!p.isAbsolute());
    EXPECT_EQ (3, p.depth());
    EXPECT_EQ ("usr", p[0]);
    EXPECT_EQ ("local", p[1]);
    EXPECT_EQ ("bin", p[2]);
    EXPECT_TRUE (!p.isDirectory());
    EXPECT_TRUE (p.isFile());
    EXPECT_EQ ("usr/local/bin/.", p.toString(Path::PATH_UNIX));
}


TEST(PathTest, testParseUnix4)
{
    Path p;
    p.parse("/usr/local/lib/../bin/", Path::PATH_UNIX);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (3, p.depth());
    EXPECT_EQ ("usr", p[0]);
    EXPECT_EQ ("local", p[1]);
    EXPECT_EQ ("bin", p[2]);
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("/usr/local/bin/", p.toString(Path::PATH_UNIX));

    p.parse("/usr/local/lib/../bin/", Path::PATH_UNIX);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (3, p.depth());
    EXPECT_EQ ("usr", p[0]);
    EXPECT_EQ ("local", p[1]);
    EXPECT_EQ ("bin", p[2]);
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("/usr/local/bin/", p.toString(Path::PATH_UNIX));

    p.parse("/usr/local/lib/../../", Path::PATH_UNIX);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (1, p.depth());
    EXPECT_EQ ("usr", p[0]);
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("/usr/", p.toString(Path::PATH_UNIX));

    p.parse("/usr/local/lib/..", Path::PATH_UNIX);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (3, p.depth());
    EXPECT_EQ ("usr", p[0]);
    EXPECT_EQ ("local", p[1]);
    EXPECT_EQ ("lib", p[2]);
    EXPECT_TRUE (!p.isDirectory());
    EXPECT_TRUE (p.isFile());
    EXPECT_EQ ("/usr/local/lib/..", p.toString(Path::PATH_UNIX));

    p.parse("../usr/local/lib/", Path::PATH_UNIX);
    EXPECT_TRUE (p.isRelative());
    EXPECT_TRUE (!p.isAbsolute());
    EXPECT_EQ (4, p.depth());
    EXPECT_EQ ("..", p[0]);
    EXPECT_EQ ("usr", p[1]);
    EXPECT_EQ ("local", p[2]);
    EXPECT_EQ ("lib", p[3]);
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("../usr/local/lib/", p.toString(Path::PATH_UNIX));

    p.parse("/usr/../lib/", Path::PATH_UNIX);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (1, p.depth());
    EXPECT_EQ ("lib", p[0]);
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("/lib/", p.toString(Path::PATH_UNIX));

    p.parse("/usr/../../lib/", Path::PATH_UNIX);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (1, p.depth());
    EXPECT_EQ ("lib", p[0]);
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("/lib/", p.toString(Path::PATH_UNIX));

    p.parse("local/../../lib/", Path::PATH_UNIX);
    EXPECT_TRUE (p.isRelative());
    EXPECT_TRUE (!p.isAbsolute());
    EXPECT_EQ (2, p.depth());
    EXPECT_EQ ("..", p[0]);
    EXPECT_EQ ("lib", p[1]);
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("../lib/", p.toString(Path::PATH_UNIX));

    p.parse("a/b/c/d", Path::PATH_UNIX);
    EXPECT_TRUE (p.isRelative());
    EXPECT_TRUE (!p.isAbsolute());
    EXPECT_EQ (3, p.depth());
    EXPECT_EQ ("a", p[0]);
    EXPECT_EQ ("b", p[1]);
    EXPECT_EQ ("c", p[2]);
    EXPECT_TRUE (!p.isDirectory());
    EXPECT_TRUE (p.isFile());
    EXPECT_EQ ("a/b/c/d", p.toString(Path::PATH_UNIX));
}


TEST(PathTest, testParseUnix5)
{
    Path p;
    p.parse("/c:/windows/system32/", Path::PATH_UNIX);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ ("c", p.getDevice());
    EXPECT_EQ (2, p.depth());
    EXPECT_EQ ("windows", p[0]);
    EXPECT_EQ ("system32", p[1]);
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("/c:/windows/system32/", p.toString(Path::PATH_UNIX));
}


TEST(PathTest, testParseWindows1)
{
    Path p;
    p.parse("", Path::PATH_WINDOWS);
    EXPECT_TRUE (p.isRelative());
    EXPECT_TRUE (!p.isAbsolute());
    EXPECT_EQ (0, p.depth());
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("", p.toString(Path::PATH_WINDOWS));

    p.parse("/", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (0, p.depth());
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("\\", p.toString(Path::PATH_WINDOWS));

    p.parse("\\", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (0, p.depth());
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("\\", p.toString(Path::PATH_WINDOWS));

    p.parse("/usr", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (0, p.depth());
    EXPECT_TRUE (!p.isDirectory());
    EXPECT_TRUE (p.isFile());
    EXPECT_EQ ("\\usr", p.toString(Path::PATH_WINDOWS));

    p.parse("\\usr", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (0, p.depth());
    EXPECT_TRUE (!p.isDirectory());
    EXPECT_TRUE (p.isFile());
    EXPECT_EQ ("\\usr", p.toString(Path::PATH_WINDOWS));

    p.parse("/usr/", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (1, p.depth());
    EXPECT_EQ ("usr", p[0]);
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("\\usr\\", p.toString(Path::PATH_WINDOWS));

    p.parse("\\usr\\", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (1, p.depth());
    EXPECT_EQ ("usr", p[0]);
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("\\usr\\", p.toString(Path::PATH_WINDOWS));
}


TEST(PathTest, testParseWindows2)
{
    Path p;
    p.parse("usr/", Path::PATH_WINDOWS);
    EXPECT_TRUE (p.isRelative());
    EXPECT_TRUE (!p.isAbsolute());
    EXPECT_EQ (1, p.depth());
    EXPECT_EQ ("usr", p[0]);
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("usr\\", p.toString(Path::PATH_WINDOWS));

    p.parse("usr", Path::PATH_WINDOWS);
    EXPECT_TRUE (p.isRelative());
    EXPECT_TRUE (!p.isAbsolute());
    EXPECT_EQ (0, p.depth());
    EXPECT_EQ ("usr", p[0]);
    EXPECT_TRUE (!p.isDirectory());
    EXPECT_TRUE (p.isFile());
    EXPECT_EQ ("usr", p.toString(Path::PATH_WINDOWS));

    p.parse("usr\\", Path::PATH_WINDOWS);
    EXPECT_TRUE (p.isRelative());
    EXPECT_TRUE (!p.isAbsolute());
    EXPECT_EQ (1, p.depth());
    EXPECT_EQ ("usr", p[0]);
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("usr\\", p.toString(Path::PATH_WINDOWS));

    p.parse("/usr/local", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (1, p.depth());
    EXPECT_EQ ("usr", p[0]);
    EXPECT_EQ ("local", p[1]);
    EXPECT_TRUE (!p.isDirectory());
    EXPECT_TRUE (p.isFile());
    EXPECT_EQ ("\\usr\\local", p.toString(Path::PATH_WINDOWS));

    p.parse("\\usr\\local", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (1, p.depth());
    EXPECT_EQ ("usr", p[0]);
    EXPECT_EQ ("local", p[1]);
    EXPECT_TRUE (!p.isDirectory());
    EXPECT_TRUE (p.isFile());
    EXPECT_EQ ("\\usr\\local", p.toString(Path::PATH_WINDOWS));

    p.parse("/usr/local/", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (2, p.depth());
    EXPECT_EQ ("usr", p[0]);
    EXPECT_EQ ("local", p[1]);
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("\\usr\\local\\", p.toString(Path::PATH_WINDOWS));

    p.parse("usr/local/", Path::PATH_WINDOWS);
    EXPECT_TRUE (p.isRelative());
    EXPECT_TRUE (!p.isAbsolute());
    EXPECT_EQ (2, p.depth());
    EXPECT_EQ ("usr", p[0]);
    EXPECT_EQ ("local", p[1]);
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("usr\\local\\", p.toString(Path::PATH_WINDOWS));

    p.parse("usr/local", Path::PATH_WINDOWS);
    EXPECT_TRUE (p.isRelative());
    EXPECT_TRUE (!p.isAbsolute());
    EXPECT_EQ (1, p.depth());
    EXPECT_EQ ("usr", p[0]);
    EXPECT_EQ ("local", p[1]);
    EXPECT_TRUE (!p.isDirectory());
    EXPECT_TRUE (p.isFile());
    EXPECT_EQ ("usr\\local", p.toString(Path::PATH_WINDOWS));

    p.parse("/usr/local/bin", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (2, p.depth());
    EXPECT_EQ ("usr", p[0]);
    EXPECT_EQ ("local", p[1]);
    EXPECT_EQ ("bin", p[2]);
    EXPECT_TRUE (!p.isDirectory());
    EXPECT_TRUE (p.isFile());
    EXPECT_EQ ("\\usr\\local\\bin", p.toString(Path::PATH_WINDOWS));

    p.parse("/usr/local/bin/", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (3, p.depth());
    EXPECT_EQ ("usr", p[0]);
    EXPECT_EQ ("local", p[1]);
    EXPECT_EQ ("bin", p[2]);
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("\\usr\\local\\bin\\", p.toString(Path::PATH_WINDOWS));

    p.parse("/usr//local/bin/", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (3, p.depth());
    EXPECT_EQ ("usr", p[0]);
    EXPECT_EQ ("local", p[1]);
    EXPECT_EQ ("bin", p[2]);
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("\\usr\\local\\bin\\", p.toString(Path::PATH_WINDOWS));

    p.parse("/usr/local//bin/", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (3, p.depth());
    EXPECT_EQ ("usr", p[0]);
    EXPECT_EQ ("local", p[1]);
    EXPECT_EQ ("bin", p[2]);
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("\\usr\\local\\bin\\", p.toString(Path::PATH_WINDOWS));

    p.parse("/usr/local/bin//", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (3, p.depth());
    EXPECT_EQ ("usr", p[0]);
    EXPECT_EQ ("local", p[1]);
    EXPECT_EQ ("bin", p[2]);
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("\\usr\\local\\bin\\", p.toString(Path::PATH_WINDOWS));
}


TEST(PathTest, testParseWindows3)
{
    Path p;
    p.parse("/usr/local/./bin/", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (3, p.depth());
    EXPECT_EQ ("usr", p[0]);
    EXPECT_EQ ("local", p[1]);
    EXPECT_EQ ("bin", p[2]);
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("\\usr\\local\\bin\\", p.toString(Path::PATH_WINDOWS));

    p.parse("./usr/local/bin/", Path::PATH_WINDOWS);
    EXPECT_TRUE (p.isRelative());
    EXPECT_TRUE (!p.isAbsolute());
    EXPECT_EQ (3, p.depth());
    EXPECT_EQ ("usr", p[0]);
    EXPECT_EQ ("local", p[1]);
    EXPECT_EQ ("bin", p[2]);
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("usr\\local\\bin\\", p.toString(Path::PATH_WINDOWS));

    p.parse("./usr/local/bin/./", Path::PATH_WINDOWS);
    EXPECT_TRUE (p.isRelative());
    EXPECT_TRUE (!p.isAbsolute());
    EXPECT_EQ (3, p.depth());
    EXPECT_EQ ("usr", p[0]);
    EXPECT_EQ ("local", p[1]);
    EXPECT_EQ ("bin", p[2]);
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("usr\\local\\bin\\", p.toString(Path::PATH_WINDOWS));

    p.parse("./usr/local/bin/.", Path::PATH_WINDOWS);
    EXPECT_TRUE (p.isRelative());
    EXPECT_TRUE (!p.isAbsolute());
    EXPECT_EQ (3, p.depth());
    EXPECT_EQ ("usr", p[0]);
    EXPECT_EQ ("local", p[1]);
    EXPECT_EQ ("bin", p[2]);
    EXPECT_TRUE (!p.isDirectory());
    EXPECT_TRUE (p.isFile());
    EXPECT_EQ ("usr\\local\\bin\\.", p.toString(Path::PATH_WINDOWS));

    p.parse("/usr/local/lib/../bin/", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (3, p.depth());
    EXPECT_EQ ("usr", p[0]);
    EXPECT_EQ ("local", p[1]);
    EXPECT_EQ ("bin", p[2]);
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("\\usr\\local\\bin\\", p.toString(Path::PATH_WINDOWS));

    p.parse("/usr/local/lib/../bin/", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (3, p.depth());
    EXPECT_EQ ("usr", p[0]);
    EXPECT_EQ ("local", p[1]);
    EXPECT_EQ ("bin", p[2]);
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("\\usr\\local\\bin\\", p.toString(Path::PATH_WINDOWS));

    p.parse("\\usr\\local\\lib\\..\\bin\\", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (3, p.depth());
    EXPECT_EQ ("usr", p[0]);
    EXPECT_EQ ("local", p[1]);
    EXPECT_EQ ("bin", p[2]);
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("\\usr\\local\\bin\\", p.toString(Path::PATH_WINDOWS));

    p.parse("/usr/local/lib/../../", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (1, p.depth());
    EXPECT_EQ ("usr", p[0]);
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("\\usr\\", p.toString(Path::PATH_WINDOWS));

    p.parse("/usr/local/lib/..", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (3, p.depth());
    EXPECT_EQ ("usr", p[0]);
    EXPECT_EQ ("local", p[1]);
    EXPECT_EQ ("lib", p[2]);
    EXPECT_TRUE (!p.isDirectory());
    EXPECT_TRUE (p.isFile());
    EXPECT_EQ ("\\usr\\local\\lib\\..", p.toString(Path::PATH_WINDOWS));

    p.parse("../usr/local/lib/", Path::PATH_WINDOWS);
    EXPECT_TRUE (p.isRelative());
    EXPECT_TRUE (!p.isAbsolute());
    EXPECT_EQ (4, p.depth());
    EXPECT_EQ ("..", p[0]);
    EXPECT_EQ ("usr", p[1]);
    EXPECT_EQ ("local", p[2]);
    EXPECT_EQ ("lib", p[3]);
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("..\\usr\\local\\lib\\", p.toString(Path::PATH_WINDOWS));

    p.parse("/usr/../lib/", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (1, p.depth());
    EXPECT_EQ ("lib", p[0]);
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("\\lib\\", p.toString(Path::PATH_WINDOWS));

    p.parse("/usr/../../lib/", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (1, p.depth());
    EXPECT_EQ ("lib", p[0]);
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("\\lib\\", p.toString(Path::PATH_WINDOWS));

    p.parse("local/../../lib/", Path::PATH_WINDOWS);
    EXPECT_TRUE (p.isRelative());
    EXPECT_TRUE (!p.isAbsolute());
    EXPECT_EQ (2, p.depth());
    EXPECT_EQ ("..", p[0]);
    EXPECT_EQ ("lib", p[1]);
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("..\\lib\\", p.toString(Path::PATH_WINDOWS));
}


TEST(PathTest, testParseWindows4)
{
    Path p;
    p.parse("\\\\server\\files", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (1, p.depth());
    EXPECT_EQ ("files", p[0]);
    EXPECT_EQ ("server", p.getNode());
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("\\\\server\\files\\", p.toString(Path::PATH_WINDOWS));

    p.parse("\\\\server\\files\\", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (1, p.depth());
    EXPECT_EQ ("files", p[0]);
    EXPECT_EQ ("server", p.getNode());
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("\\\\server\\files\\", p.toString(Path::PATH_WINDOWS));

    p.parse("\\\\server\\files\\file", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (1, p.depth());
    EXPECT_EQ ("files", p[0]);
    EXPECT_EQ ("server", p.getNode());
    EXPECT_TRUE (!p.isDirectory());
    EXPECT_TRUE (p.isFile());
    EXPECT_EQ ("\\\\server\\files\\file", p.toString(Path::PATH_WINDOWS));

    p.parse("\\\\server\\files\\dir\\file", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (2, p.depth());
    EXPECT_EQ ("files", p[0]);
    EXPECT_EQ ("dir", p[1]);
    EXPECT_EQ ("server", p.getNode());
    EXPECT_TRUE (!p.isDirectory());
    EXPECT_TRUE (p.isFile());
    EXPECT_EQ ("\\\\server\\files\\dir\\file", p.toString(Path::PATH_WINDOWS));

    p.parse("\\\\server\\files\\dir\\file", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (2, p.depth());
    EXPECT_EQ ("files", p[0]);
    EXPECT_EQ ("dir", p[1]);
    EXPECT_EQ ("server", p.getNode());
    EXPECT_TRUE (!p.isDirectory());
    EXPECT_TRUE (p.isFile());
    EXPECT_EQ ("\\\\server\\files\\dir\\file", p.toString(Path::PATH_WINDOWS));	

    p.parse("\\\\server", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (0, p.depth());
    EXPECT_EQ ("server", p.getNode());
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("\\\\server\\", p.toString(Path::PATH_WINDOWS));

    p.parse("c:\\", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (0, p.depth());
    EXPECT_EQ ("c", p.getDevice());
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("c:\\", p.toString(Path::PATH_WINDOWS));	

    p.parse("c:\\WinNT", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (0, p.depth());
    EXPECT_EQ ("c", p.getDevice());
    EXPECT_TRUE (!p.isDirectory());
    EXPECT_TRUE (p.isFile());
    EXPECT_EQ ("c:\\WinNT", p.toString(Path::PATH_WINDOWS));	

    p.parse("c:\\WinNT\\", Path::PATH_WINDOWS);
    EXPECT_TRUE (!p.isRelative());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_EQ (1, p.depth());
    EXPECT_EQ ("WinNT", p[0]);
    EXPECT_EQ ("c", p.getDevice());
    EXPECT_TRUE (p.isDirectory());
    EXPECT_TRUE (!p.isFile());
    EXPECT_EQ ("c:\\WinNT\\", p.toString(Path::PATH_WINDOWS));

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
    EXPECT_EQ (3, p.depth());
    EXPECT_EQ ("a", p[0]);
    EXPECT_EQ ("b", p[1]);
    EXPECT_EQ ("c", p[2]);
    EXPECT_TRUE (!p.isDirectory());
    EXPECT_TRUE (p.isFile());
    EXPECT_EQ ("a\\b\\c\\d", p.toString(Path::PATH_WINDOWS));
}
TEST(PathTest, testTryParse)
{
    Path p;
    EXPECT_TRUE (p.tryParse("/etc/passwd"));
    EXPECT_EQ ("/etc/passwd", p.toString());

    EXPECT_TRUE (p.tryParse("c:\\windows\\system", Path::PATH_WINDOWS));
    EXPECT_EQ ("c:\\windows\\system", p.toString(Path::PATH_WINDOWS));
    EXPECT_TRUE (!p.tryParse("c:foo.bar", Path::PATH_WINDOWS));
    EXPECT_EQ ("c:\\windows\\system", p.toString(Path::PATH_WINDOWS));
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
    EXPECT_EQ ("foo.bar", p.getFileName());
    EXPECT_EQ ("foo", p.getBaseName());
    EXPECT_EQ ("bar", p.getExtension());

    p.setBaseName("readme");
    EXPECT_EQ ("readme.bar", p.getFileName());
    EXPECT_EQ ("readme", p.getBaseName());
    EXPECT_EQ ("bar", p.getExtension());

    p.setExtension("txt");
    EXPECT_EQ ("readme.txt", p.getFileName());
    EXPECT_EQ ("readme", p.getBaseName());
    EXPECT_EQ ("txt", p.getExtension());

    p.setExtension("html");
    EXPECT_EQ ("readme.html", p.getFileName());
    EXPECT_EQ ("readme", p.getBaseName());
    EXPECT_EQ ("html", p.getExtension());

    p.setBaseName("index");
    EXPECT_EQ ("index.html", p.getFileName());
    EXPECT_EQ ("index", p.getBaseName());
    EXPECT_EQ ("html", p.getExtension());
}


TEST(PathTest, testAbsolute)
{
    Path base("C:\\Program Files\\", Path::PATH_WINDOWS);
    Path rel("lyx");
    Path abs = rel.absolute(base);
    EXPECT_EQ ("C:\\Program Files\\lyx", abs.toString(Path::PATH_WINDOWS));

    base.parse("/usr/local", Path::PATH_UNIX);
    rel.parse("lyx/include", Path::PATH_UNIX);
    abs = rel.absolute(base);
    EXPECT_EQ ("/usr/local/lyx/include", abs.toString(Path::PATH_UNIX));

    base.parse("/usr/local/bin", Path::PATH_UNIX);
    rel.parse("../lyx/include", Path::PATH_UNIX);
    abs = rel.absolute(base);
    EXPECT_EQ ("/usr/local/lyx/include", abs.toString(Path::PATH_UNIX));
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
    EXPECT_EQ ("/usr/local/", p.toString(Path::PATH_UNIX));
    p.makeParent();
    EXPECT_EQ ("/usr/", p.toString(Path::PATH_UNIX));
    p.makeParent();
    EXPECT_EQ ("/", p.toString(Path::PATH_UNIX));
    p.makeParent();
    EXPECT_EQ ("/", p.toString(Path::PATH_UNIX));
}


TEST(PathTest, testForDirectory)
{
    Path p = Path::forDirectory("/usr/local/include", Path::PATH_UNIX);
    EXPECT_EQ ("/usr/local/include/", p.toString(Path::PATH_UNIX));

    p = Path::forDirectory("/usr/local/include/", Path::PATH_UNIX);
    EXPECT_EQ ("/usr/local/include/", p.toString(Path::PATH_UNIX));
}


TEST(PathTest, testExpand)
{
    std::string s = Path::expand("~/.bashrc");
    /*
    EXPECT_EQ (Path::expand("$HOME/.bashrc"), s);
    EXPECT_TRUE (s == Environment::get("HOME") + "/.bashrc" || 
            s == Environment::get("HOME") + "//.bashrc");
            */
    Path p(s);
    /*
    s = Path::expand("$HOME/.bashrc");
    EXPECT_EQ (Path::expand("~/.bashrc"), s);
    */
    s = Path::expand("${HOME}/.bashrc");
    EXPECT_EQ (Path::expand("~/.bashrc"), s);
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
    EXPECT_EQ ("\\\\server\\files\\foo.bar", p1.toString());
    EXPECT_EQ ("c:\\temp\\foo.bar", p2.toString());
}


TEST(PathTest, testResolve)
{
    Path p("c:\\foo\\", Path::PATH_WINDOWS);
    p.resolve("test.dat");
    EXPECT_EQ ("c:\\foo\\test.dat", p.toString(Path::PATH_WINDOWS));

    p.assign("c:\\foo\\", Path::PATH_WINDOWS);
    p.resolve(Path("d:\\bar.txt", Path::PATH_WINDOWS));
    EXPECT_EQ ("d:\\bar.txt", p.toString(Path::PATH_WINDOWS));

    p.assign("c:\\foo\\bar.txt", Path::PATH_WINDOWS);
    p.resolve("foo.txt");
    EXPECT_EQ ("c:\\foo\\foo.txt", p.toString(Path::PATH_WINDOWS));

    p.assign("c:\\foo\\bar\\", Path::PATH_WINDOWS);
    p.resolve(Path("..\\baz\\test.dat", Path::PATH_WINDOWS));
    EXPECT_EQ ("c:\\foo\\baz\\test.dat", p.toString(Path::PATH_WINDOWS));
}


TEST(PathTest, testPushPop)
{
    Path p;
    p.pushDirectory("a");
    p.pushDirectory("b");
    p.pushDirectory("c");
    EXPECT_EQ ("a/b/c/", p.toString(Path::PATH_UNIX));

    p.popDirectory();
    EXPECT_EQ ("a/b/", p.toString(Path::PATH_UNIX));

    p.popFrontDirectory();
    EXPECT_EQ ("b/", p.toString(Path::PATH_UNIX));
}
