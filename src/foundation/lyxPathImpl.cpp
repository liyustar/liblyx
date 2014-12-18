#include "lyxPathImpl.h"
#include "lyxException.h"
#include <unistd.h>

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
    throw NotImplementedException();
}

std::string PathImpl::tempImpl() {
    throw NotImplementedException();
}

std::string PathImpl::nullImpl() {
    throw NotImplementedException();
}

std::string PathImpl::expandImpl(const std::string& path) {
    throw NotImplementedException();
}

} // namespace lyx
