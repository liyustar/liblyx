#include "lyxDirectoryIteratorImpl.h"
#include "lyxFile.h"
#include "lyxPath.h"

namespace lyx {

DirectoryIteratorImpl::DirectoryIteratorImpl(const std::string& path): _pDir(0), _rc(1) {
    Path p(path);
    p.makeFile();

    _pDir = opendir(p.toString().c_str());
    if (!_pDir) File::handleLastError(path);

    next();
}

DirectoryIteratorImpl::~DirectoryIteratorImpl() {
    if (_pDir) closedir(_pDir);
}

const std::string& DirectoryIteratorImpl::next() {
    do {
        struct dirent* pEntry = readdir(_pDir);
        if (pEntry)
            _current = pEntry->d_name;
        else
            _current.clear();
    } while (_current == "." || _current == "..");
    return _current;
}

} // namespace lyx
