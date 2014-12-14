#ifndef LIBLYX_FOUNDATION_LYXFILE_H_
#define LIBLYX_FOUNDATION_LYXFILE_H_

#include "lyxFileImpl.h"

namespace lyx {

class File: public FileImpl {
    public:
        static void handleLastError(const std::string& path);
};

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXFILE_H_
