#ifndef LIBLYX_FOUNDATION_LYXFILEIMPL_H_
#define LIBLYX_FOUNDATION_LYXFILEIMPL_H_

#include "lyxTimestamp.h"
#include <string>

namespace lyx {

class FileImpl {
    protected:
        typedef uint64_t FileSizeImpl;

        FileImpl();
        FileImpl(const std::string& path);
        virtual ~FileImpl();
        void swapImpl(FileImpl& file);
        void setPathImpl(const std::string& path);
        const std::string& getPathImpl() const;
        bool existsImpl() const;
        bool canReadImpl() const;
        bool canWriteImpl() const;
        bool canExecuteImpl() const;
        bool isFileImpl() const;
        bool isDirectoryImpl() const;
        bool isLinkImpl() const;
        bool isDeviceImpl() const;
        bool isHiddenImpl() const;
        Timestamp createdImpl() const;
        Timestamp getLastModifiedImpl() const;
        void setLastModifiedImpl(const Timestamp& ts);
        FileSizeImpl getSizeImpl() const;
        void setSizeImpl(FileSizeImpl size);
        void setWriteableImpl(bool flag = true);
        void setExecutableImpl(bool flag = true);
        void copyToImpl(const std::string& path) const;
        void renameToImpl(const std::string& path);
        void removeImpl();
        bool createFileImpl();
        bool createDirecotryImpl();
        static void handleLastErrorImpl(const std::string& path);

    private:
        std::string _path;
};

inline const std::string& FileImpl::getPathImpl() const {
    return _path;
}

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXFILEIMPL_H_
