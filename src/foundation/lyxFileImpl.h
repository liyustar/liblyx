#ifndef LIBLYX_FOUNDATION_LYXFILEIMPL_H_
#define LIBLYX_FOUNDATION_LYXFILEIMPL_H_

#include <string>

namespace lyx {

class FileImpl {
    protected:
        static void handleLastErrorImpl(const std::string& path);
};

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXFILEIMPL_H_
