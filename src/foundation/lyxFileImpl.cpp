#include "lyxFileImpl.h"
#include "lyxException.h"
#include <errno.h>
#include <cstring>

namespace lyx {

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
