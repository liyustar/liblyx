#ifndef LIBLYX_FOUNDATION_LYXPATH_H_
#define LIBLYX_FOUNDATION_LYXPATH_H_

#include <string>
#include <vector>

namespace lyx {

class Path {
    public:
        enum Style {
            PATH_UNIX,
            PATH_WINDOWS
        };

        typedef std::vector<std::string> StringVec;

        Path();
        Path(bool absolute);
        Path(const char* path);
        Path(const char* path, Style style);
        Path(const std::string& path);
        Path(const std::string& path, Style style);
        Path(const Path& path);
        Path(const Path& parent, const std::string& fileName);
        Path(const Path& parent, const char* fileName);
        Path(const Path& parent, const Path& relative);
        ~Path();

        Path& operator = (const Path& path);
        Path& operator = (const std::string& path);
        Path& operator = (const char* path);

        void swap(Path& path);

        Path& assign(const char* path);
        Path& assign(const std::string& path);
        Path& assign(const std::string& path, Style style);
        Path& assign(const Path& path);

        std::string toString() const;
        std::string toString(Style style) const;

        Path& parse(const std::string& path);
        Path& parse(const std::string& path, Style style);
        bool tryParse(const std::string& path);
        bool tryParse(const std::string& path, Style style);

        Path& parseDirectory(const std::string& path);
        Path& parseDirectory(const std::string& path, Style style);

        Path& makeDirectory();
        Path& makeFile();
        Path& makeParent();
        Path& makeAbsolute();
        Path& makeAbsolute(const Path& base);

        Path& append(const Path& path);
        Path& resolve(const Path& path);

        bool isAbsolute() const;
        bool isRelative() const;
        bool isDirectory() const;
        bool isFile() const;

        Path& setNode(const std::string& node);
        const std::string& getNode() const;
        Path& setDevice(const std::string& device);
        const std::string& getDevice() const;

        int depth() const;
        const std::string& directory(int n) const;
        const std::string& operator [] (int n) const;
        Path& pushDirectory(const std::string& dir);
        Path& popDirectory();
        Path& popFrontDirectory();

        Path& setFileName(const std::string& name);
        const std::string& getFileName() const;
        Path& setBaseName(const std::string& name);
        std::string getBaseName() const;
        Path& setExtension(const std::string& extension);
        std::string getExtension() const;

        Path& clear();
        Path parent() const;
        Path absolute() const;
        Path absolute(const Path& base) const;

        static Path forDirectory(const std::string& path);
        static Path forDirectory(const std::string& path, Style style);
        static char separator();
        static char pathSeparator();
        static std::string current();
        static std::string home();
        static std::string temp();
        static std::string null();
        static std::string expand(const std::string& path);
        static void listRoots(std::vector<std::string>& roots);
        static bool find(StringVec::const_iterator it, StringVec::const_iterator end, const std::string& name, Path& path);
        static bool find(const std::string& pathList, const std::string& name, Path& path);

    protected:
        void parseUnix(const std::string& path);
        void parseWindows(const std::string& path);
        std::string buildUnix() const;
        std::string buildWindows() const;

    private:
        std::string _node;
        std::string _device;
        std::string _name;
        StringVec   _dirs;
        bool        _absolute;
};

inline bool Path::isAbsolute() const {
    return _absolute;
}

inline bool Path::isRelative() const {
    return !_absolute;
}

inline bool Path::isDirectory() const {
    return _name.empty();
}

inline bool Path::isFile() const {
    return !_name.empty();
}

inline Path& Path::parse(const std::string& path) {
    return assign(path);
}

inline Path& Path::parse(const std::string& path, Style style) {
    return assign(path, style);
}

inline const std::string& Path::getNode() const {
    return _node;
}

inline const std::string& Path::getDevice() const {
    return _device;
}

inline const std::string& Path::getFileName() const {
    return _name;
}

inline int Path::depth() const {
    return int(_dirs.size());
}

inline Path Path::forDirectory(const std::string& path) {
    Path p;
    return p.parseDirectory(path);
}

inline Path Path::forDirectory(const std::string& path, Style style) {
    Path p;
    return p.parseDirectory(path, style);
}

inline char Path::separator() {
    return '/';
}

inline char Path::pathSeparator() {
    return ':';
}

inline void swap(Path& p1, Path& p2) {
    p1.swap(p2);
}

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXPATH_H_
