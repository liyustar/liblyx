#include "lyxFileImpl.h"
#include "lyxBuffer.h"
#include "lyxException.h"
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <utime.h>
#include <utility>
#include <cstring>
#include <cassert>

namespace lyx {

FileImpl::FileImpl() {
}

FileImpl::FileImpl(const std::string& path): _path(path) {
    std::string::size_type n = _path.size();
    if (n > 1 && _path[n - 1] == '/')
        _path.resize(n - 1);
}

FileImpl::~FileImpl() {
}

void FileImpl::swapImpl(FileImpl& file) {
    std::swap(_path, file._path);
}

void FileImpl::setPathImpl(const std::string& path) {
    _path = path;
    std::string::size_type n = _path.size();
    if (n > 1 && _path[n - 1] == '/')
        _path.resize(n - 1);
}

bool FileImpl::existsImpl() const {
    assert (!_path.empty());

    struct stat st;
    return stat(_path.c_str(), &st) == 0;
}

bool FileImpl::canReadImpl() const {
    assert (!_path.empty());

    struct stat st;
    if (stat(_path.c_str(), &st) == 0) {
        if (st.st_uid == geteuid())
            return (st.st_mode & S_IRUSR) != 0;
        else if(st.st_gid == getegid())
            return (st.st_mode & S_IRGRP) != 0;
        else
            return (st.st_mode & S_IROTH) != 0;
    }
    else handleLastErrorImpl(_path);
    return false;
}

bool FileImpl::canWriteImpl() const {
    assert (!_path.empty());

    struct stat st;
    if (stat(_path.c_str(), &st) == 0) {
        if (st.st_uid == geteuid())
            return (st.st_mode & S_IWUSR) != 0;
        else if(st.st_gid == getegid())
            return (st.st_mode & S_IWGRP) != 0;
        else
            return (st.st_mode & S_IWOTH) != 0;
    }
    else handleLastErrorImpl(_path);
    return false;
}

bool FileImpl::canExecuteImpl() const {
    assert (!_path.empty());

    struct stat st;
    if (stat(_path.c_str(), &st) == 0) {
        if (st.st_uid == geteuid())
            return (st.st_mode & S_IXUSR) != 0;
        else if(st.st_gid == getegid())
            return (st.st_mode & S_IXGRP) != 0;
        else
            return (st.st_mode & S_IXOTH) != 0;
    }
    else handleLastErrorImpl(_path);
    return false;
}

bool FileImpl::isFileImpl() const {
    assert (!_path.empty());

    struct stat st;
    if (stat(_path.c_str(), &st) == 0)
        return S_ISREG(st.st_mode);
    else
        handleLastErrorImpl(_path);
    return false;
}

bool FileImpl::isDirectoryImpl() const {
    assert (!_path.empty());

    struct stat st;
    if (stat(_path.c_str(), &st) == 0)
        return S_ISDIR(st.st_mode);
    else
        handleLastErrorImpl(_path);
    return false;
}

bool FileImpl::isLinkImpl() const {
    assert (!_path.empty());

    struct stat st;
    if (stat(_path.c_str(), &st) == 0)
        return S_ISLNK(st.st_mode);
    else
        handleLastErrorImpl(_path);
    return false;
}

bool FileImpl::isDeviceImpl() const {
    assert (!_path.empty());

    struct stat st;
    if (stat(_path.c_str(), &st) == 0)
        return S_ISCHR(st.st_mode) || S_ISBLK(st.st_mode);
    else
        handleLastErrorImpl(_path);
    return false;
}

bool FileImpl::isHiddenImpl() const {
    assert (!_path.empty());

    throw NotImplementedException();
    /*
    Path p(_path);
    p.makeFile();

    return p.getFileName()[0] == '.';
    */
}

Timestamp FileImpl::createdImpl() const {
    assert (!_path.empty());

    struct stat st;
    if (stat(_path.c_str(), &st) == 0)
        return Timestamp::fromEpochTime(st.st_ctime);
    else
        handleLastErrorImpl(_path);
    return 0;
}

Timestamp FileImpl::getLastModifiedImpl() const {
    assert (!_path.empty());

    struct stat st;
    if (stat(_path.c_str(), &st) == 0)
        return Timestamp::fromEpochTime(st.st_mtime);
    else
        handleLastErrorImpl(_path);
    return 0;
}

void FileImpl::setLastModifiedImpl(const Timestamp& ts) {
    assert (!_path.empty());

    struct utimbuf tb;
    tb.actime  = ts.epochTime();
    tb.modtime = ts.epochTime();
    if (utime(_path.c_str(), &tb) != 0)
        handleLastErrorImpl(_path);
}

FileImpl::FileSizeImpl FileImpl::getSizeImpl() const {
    assert (!_path.empty());

    struct stat st;
    if (stat(_path.c_str(), &st) == 0)
        return st.st_size;
    else
        handleLastErrorImpl(_path);
    return 0;
}
void FileImpl::setSizeImpl(FileSizeImpl size) {
    assert (!_path.empty());

    if (truncate(_path.c_str(), size) != 0)
        handleLastErrorImpl(_path);
}

void FileImpl::setWriteableImpl(bool flag) {
    assert (!_path.empty());

    struct stat st;
    if (stat(_path.c_str(), &st) != 0)
        handleLastErrorImpl(_path);
    mode_t mode;
    if (flag) {
        mode = st.st_mode | S_IWUSR;
    }
    else {
        mode_t wmask = S_IWUSR | S_IWGRP | S_IWOTH;
        mode = st.st_mode & ~wmask;
    }
    if (chmod(_path.c_str(), mode) != 0)
        handleLastErrorImpl(_path);
}

void FileImpl::setExecutableImpl(bool flag) {
    assert (!_path.empty());

    struct stat st;
    if (stat(_path.c_str(), &st) != 0)
        handleLastErrorImpl(_path);
    mode_t mode;
    if (flag) {
        mode = st.st_mode | S_IXUSR;
    }
    else {
        mode_t wmask = S_IXUSR | S_IXGRP | S_IXOTH;
        mode = st.st_mode & ~wmask;
    }
    if (chmod(_path.c_str(), mode) != 0)
        handleLastErrorImpl(_path);
}

void FileImpl::copyToImpl(const std::string& path) const {
    assert (!_path.empty());

    throw NotImplementedException();
}

void FileImpl::renameToImpl(const std::string& path) {
    assert (!_path.empty());

    if (rename(_path.c_str(), path.c_str()) != 0)
        handleLastErrorImpl(_path);
}

void FileImpl::removeImpl() {
    assert (!_path.empty());

    int rc;
    if (!isLinkImpl() && isDirectoryImpl())
        rc = rmdir(_path.c_str());
    else
        rc = unlink(_path.c_str());
    if (rc) handleLastErrorImpl(_path);
}


bool FileImpl::createFileImpl() {
    assert (!_path.empty());

    const int    DEFAULT_CREATEFILE_FLAG = O_WRONLY | O_CREAT | O_EXCL;
    const mode_t DEFAULT_CREATEFILE_MODE = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    int n = open(_path.c_str(), DEFAULT_CREATEFILE_FLAG, DEFAULT_CREATEFILE_MODE);
    if (n != -1) {
        close(n);
        return true;
    }
    if (n == -1 && errno == EEXIST)
        return false;
    else
        handleLastErrorImpl(_path);
    return false;
}

bool FileImpl::createDirectoryImpl() {
    assert (!_path.empty());

    const mode_t DEFAULT_CREATEDIR_MODE = S_IRWXU | S_IRWXG | S_IRWXO;
    if (existsImpl() && isDirectoryImpl())
        return false;
    if (mkdir(_path.c_str(), DEFAULT_CREATEDIR_MODE) != 0)
        handleLastErrorImpl(_path);
    return true;
}

void FileImpl::handleLastErrorImpl(const std::string& path) {
    switch (errno) {
        case EIO:
            throw IOException(path, errno);
        case EPERM:
            throw FileAccessDeniedException("insufficient permissions", path, errno);
        case EACCES:
            throw FileAccessDeniedException(path, errno);
        case ENOENT:
            throw FileNotFoundException(path, errno);
        case ENOTDIR:
            throw OpenFileException("not a directory", path, errno);
        case EISDIR:
            throw OpenFileException("not a file", path, errno);
        case EROFS:
            throw FileReadOnlyException(path, errno);
        case EEXIST:
            throw FileExistsException(path, errno);
        case ENOSPC:
            throw FileException("no space left on device", path, errno);
        case EDQUOT:
            throw FileException("disk quota exceeded", path, errno);
        case ENOTEMPTY:
            throw FileException("directory not empty", path, errno);
        case ENAMETOOLONG:
            throw PathSyntaxException(path, errno);
        case ENFILE:
        case EMFILE:
            throw FileException("too many open files", path, errno);
        default:
            throw FileException(std::strerror(errno), path, errno);
    }
}

} // namespace lyx
