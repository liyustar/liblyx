#ifndef LIBLYX_FOUNDATION_LYXFILE_H_
#define LIBLYX_FOUNDATION_LYXFILE_H_

#include "lyxFileImpl.h"
#include "lyxTimestamp.h"
#include <vector>

namespace lyx {

class Path;

class File: public FileImpl {
    public:
        typedef FileSizeImpl FileSize;

        File();
        File(const std::string& path);
        File(const char* path);
        File(const Path& path);
        File(const File& file);
        virtual ~File();

        File& operator = (const File& file);
        File& operator = (const std::string& path);
        File& operator = (const char* path);
        File& operator = (const Path& path);

        void swap(File& file);
        const std::string& path() const;
        bool exists() const;
        bool canRead() const;
        bool canWrite() const;
        bool canExecute() const;

        bool isFile() const;
        bool isLink() const;
        bool isDirectory() const;
        bool isDevice() const;
        bool isHidden() const;

        Timestamp created() const;
        Timestamp getLastModified() const;
        File& setLastModified(const Timestamp& ts);

        FileSize getSize() const;
        File& setSize(FileSize size);
        File& setWriteable(bool flag = true);
        File& setReadOnly(bool flag = true);
        File& setExecutable(bool flag = true);

        void copyTo(const std::string& path) const;
        void moveTo(const std::string& path);
        void renameTo(const std::string& path);
        void remove(bool recursive = false);

        bool createFile();
        bool createDirectory();
        void createDirectories();
        void list(std::vector<std::string>& files) const;
        void list(std::vector<File>& files) const;

        bool operator == (const File& file) const;
        bool operator != (const File& file) const;
        bool operator <  (const File& file) const;
        bool operator <= (const File& file) const;
        bool operator >  (const File& file) const;
        bool operator >= (const File& file) const;

        static void handleLastError(const std::string& path);

    protected:
        void copyDirecotry(const std::string& path) const;
};

inline const std::string& File::path() const {
    return getPathImpl();
}

inline bool File::operator == (const File& file) const {
    return getPathImpl() == file.getPathImpl();
}

inline bool File::operator != (const File& file) const {
    return getPathImpl() != file.getPathImpl();
}

inline bool File::operator < (const File& file) const {
    return getPathImpl() < file.getPathImpl();
}

inline bool File::operator <= (const File& file) const {
    return getPathImpl() <= file.getPathImpl();
}

inline bool File::operator > (const File& file) const {
    return getPathImpl() > file.getPathImpl();
}

inline bool File::operator >= (const File& file) const {
    return getPathImpl() >= file.getPathImpl();
}

inline void swap(File& f1, File& f2) {
    f1.swap(f2);
}

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXFILE_H_
