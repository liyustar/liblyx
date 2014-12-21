#include "lyxFile.h"
#include "lyxDirectoryIterator.h"
#include "lyxException.h"
#include <cassert>

namespace lyx {

File::File() {
}

File::File(const std::string& path): FileImpl(path) {
}

File::File(const char* path): FileImpl(std::string(path)) {
}

File::File(const Path& path): FileImpl(path.toString()) {
}

File::File(const File& file): FileImpl(file.getPathImpl()) {
}

File::~File() {
}

File& File::operator = (const File& file) {
    setPathImpl(file.getPathImpl());
    return *this;
}

File& File::operator = (const char* path) {
    assert (path);
    setPathImpl(path);
    return *this;
}

File& File::operator = (const Path& path) {
    setPathImpl(path.toString());
    return *this;
}

void File::swap(File& file) {
    swapImpl(file);
}

bool File::exists() const {
    return existsImpl();
}

bool File::canRead() const {
    return canReadImpl();
}

bool File::canWrite() const {
    return canWriteImpl();
}

bool File::canExecute() const {
    return canExecuteImpl();
}

bool File::isFile() const {
    return isFileImpl();
}

bool File::isLink() const {
    return isLinkImpl();
}

bool File::isDirectory() const {
    return isDirectoryImpl();
}

bool File::isDevice() const {
    return isDeviceImpl();
}

bool File::isHidden() const {
    return isHiddenImpl();
}

Timestamp File::created() const {
    return createdImpl();
}

Timestamp File::getLastModified() const {
    return getLastModifiedImpl();
}

File& File::setLastModified(const Timestamp& ts) {
    setLastModifiedImpl(ts);
    return *this;
}

File::FileSize File::getSize() const {
    return getSizeImpl();
}

File& File::setSize(FileSize size) {
    setSizeImpl(size);
    return *this;
}

File& File::setWriteable(bool flag) {
    setWriteableImpl(flag);
    return *this;
}

File& File::setReadOnly(bool flag) {
    setWriteableImpl(!flag);
    return *this;
}

File& File::setExecutable(bool flag) {
    setExecutableImpl(flag);
    return *this;
}

void File::copyTo(const std::string& path) const {
    Path src(getPathImpl());
    Path dest(path);
    File destFile(path);
    if ((destFile.exists() && destFile.isDirectory()) || dest.isDirectory()) {
        dest.makeDirectory();
        dest.setFileName(src.getFileName());
    }
    if (isDirectory())
        copyDirecotry(dest.toString());
    else
        copyToImpl(dest.toString());
}

void File::copyDirecotry(const std::string& path) const {
    File target(path);
    target.createDirectories();

    Path src(getPathImpl());
    src.makeFile();
    DirectoryIterator it(src);
    DirectoryIterator end;
    for (; it != end; ++it) {
        it->copyTo(path);
    }
}

void File::moveTo(const std::string& path) {
    copyTo(path);
    remove(true);
    setPathImpl(path);
}

void File::renameTo(const std::string& path) {
    renameToImpl(path);
    setPathImpl(path);
}

void File::remove(bool recursive) {
    if (recursive && !isLink() && isDirectory()) {
        std::vector<File> files;
        list(files);
        for (std::vector<File>::iterator it = files.begin(); it != files.end(); it++) {
            it->remove(true);
        }
    }
    removeImpl();
}

bool File::createFile() {
    return createFileImpl();
}

bool File::createDirectory() {
    return createDirectoryImpl();
}

void File::createDirectories() {
    if (!exists()) {
        Path p(getPathImpl());
        p.makeDirectory();
        if (p.depth() > 1) {
            p.makeParent();
            File f(p);
            f.createDirectories();
        }
        createDirectoryImpl();
    }
}

void File::list(std::vector<std::string>& files) const {
    files.clear();
    DirectoryIterator it(*this);
    DirectoryIterator end;
    while (it != end) {
        files.push_back(it.name());
        ++it;
    }
}

void File::list(std::vector<File>& files) const {
    files.clear();
    DirectoryIterator it(*this);
    DirectoryIterator end;
    while (it != end) {
        files.push_back(*it);
        ++it;
    }
}

void File::handleLastError(const std::string& path) {
    handleLastErrorImpl(path);
}

} // namespace lyx
