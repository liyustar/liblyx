#ifndef LIBLYX_FOUNDATION_LYXDIRECTORYITERATOR_H_
#define LIBLYX_FOUNDATION_LYXDIRECTORYITERATOR_H_

#include "lyxFile.h"
#include "lyxPath.h"

namespace lyx {

class DirectoryIteratorImpl;

class DirectoryIterator {
    public:
        DirectoryIterator();
        DirectoryIterator(const std::string& path);
        DirectoryIterator(const DirectoryIterator& iterator);
        DirectoryIterator(const File& file);
        DirectoryIterator(const Path& path);
        virtual ~DirectoryIterator();

        const std::string& name() const;
        const Path& path() const;

        DirectoryIterator& operator = (const DirectoryIterator& it);
        DirectoryIterator& operator = (const File& file);
        DirectoryIterator& operator = (const Path& path);
        DirectoryIterator& operator = (const std::string& path);

        virtual DirectoryIterator& operator ++ (); // prefix

        const File& operator * () const;
        File& operator * ();
        const File* operator -> () const;
        File* operator -> ();

        bool operator == (const DirectoryIterator& iterator) const;
        bool operator != (const DirectoryIterator& iterator) const;

    protected:
        Path _path;
        File _file;

    private:
        DirectoryIteratorImpl* _pImpl;
};

inline const std::string& DirectoryIterator::name() const {
    return _path.getFileName();
}

inline const Path& DirectoryIterator::path() const {
    return _path;
}

inline const File& DirectoryIterator::operator * () const {
    return _file;
}

inline File& DirectoryIterator::operator * () {
    return _file;
}

inline const File* DirectoryIterator::operator -> () const {
    return &_file;
}

inline File* DirectoryIterator::operator -> () {
    return &_file;
}

inline bool DirectoryIterator::operator != (const DirectoryIterator& iterator) const {
    return name() != iterator.name();
}

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXDIRECTORYITERATOR_H_
