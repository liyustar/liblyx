#include "lyxPathImpl.h"
#include "lyxException.h"
#include <cstdlib>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>

#ifndef PATH_MAX
#define PATH_MAX 1024 // fallback
#endif

namespace lyx {

std::string PathImpl::currentImpl() {
    std::string path;
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)))
        path = cwd;
    else
        throw SystemException("cannot get current directory");
    std::string::size_type n = path.size();
    if (n > 0 && path[n - 1] != '/') path.append("/");
    return path;
}

std::string PathImpl::homeImpl() {
    std::string path;
    struct passwd* pwd = getpwuid(getuid());
    if (pwd)
        path = pwd->pw_dir;
    else {
        pwd = getpwuid(geteuid());
        if (pwd)
            path = pwd->pw_dir;
        else
            // path = EnvironmentImpl::getImpl("HOME");
            throw NotImplementedException();
    }
    std::string::size_type n = path.size();
    if (n > 0 && path[n - 1] != '/') path.append("/");
    return path;
}

std::string PathImpl::tempImpl() {
    std::string path;
    char* tmp = std::getenv("TMPDIR");
    if (tmp) {
        path = tmp;
        std::string::size_type n = path.size();
        if (n > 0 && path[n - 1] != '/')
            path.append("/");
    }
    else {
        path = "/tmp/";
    }
    return path;
}

std::string PathImpl::nullImpl() {
    return "/dev/null";
}

std::string PathImpl::expandImpl(const std::string& path) {
    std::string result;
    std::string::const_iterator it  = path.begin();
    std::string::const_iterator end = path.end();
    if (it != end && *it == '~') {
        it++;
        if (it != end && *it == '/') {
            result += homeImpl();
            it++;
        }
        else result += '~';
    }
    while (it != end) {
        if (*it == '$') {
            std::string var;
            it++;
            if (it != end && *it == '{') {
                it++;
                while (it != end && *it != '}') var += *it++;
                if (it != end) it++;
            }
            else {
                // while (it != end && (Ascii::isAlphaNumeric(*it) || *it == '_')) var += *it++;
                throw NotImplementedException();
            }
            char* val = std::getenv(var.c_str());
            if (val) result += val;
        }
        else result += *it++;
    }
    return result;
}

void PathImpl::listRootsImpl(std::vector<std::string>& roots) {
    roots.clear();
    roots.push_back("/");
}

} // namespace lyx
