#ifndef LIBLYX_FOUNDATION_LYXDIRECTORYITERATORIMPL_H_
#define LIBLYX_FOUNDATION_LYXDIRECTORYITERATORIMPL_H_

#include <string>
#include <dirent.h>

namespace lyx {

class DirectoryIteratorImpl {
    public:
        DirectoryIteratorImpl(const std::string& path);
        ~DirectoryIteratorImpl();

        void duplicate();
        void release();

        const std::string& get() const;
        const std::string& next();

    private:
        DIR*        _pDir;
        std::string _current;
        int         _rc;
};

const std::string& DirectoryIteratorImpl::get() const {
    return _current;
}

inline void DirectoryIteratorImpl::duplicate() {
    _rc++;
}

inline void DirectoryIteratorImpl::release() {
    if (--_rc == 0)
        delete this;
}

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXDIRECTORYITERATORIMPL_H_
