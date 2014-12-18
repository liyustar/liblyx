#ifndef LIBLYX_FOUNDATION_LYXPATHIMPL_H_
#define LIBLYX_FOUNDATION_LYXPATHIMPL_H_

#include <string>

namespace lyx {

class PathImpl {
    public:
        static std::string currentImpl();
        static std::string homeImpl();
        static std::string tempImpl();
        static std::string nullImpl();
        static std::string expandImpl(const std::string& path);
};

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXPATHIMPL_H_
