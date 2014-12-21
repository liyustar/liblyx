#ifndef LIBLYX_FOUNDATION_LYXTEMPORARYFILE_H_
#define LIBLYX_FOUNDATION_LYXTEMPORARYFILE_H_

#include "lyxFile.h"
#include <string>

namespace lyx {

class TemporaryFile: public File {
    public:
        TemporaryFile();
        TemporaryFile(const std::string& tempDir);
        ~TemporaryFile();

        void keep();
        void keepUntilExit();
        static void registerForDeletion(const std::string& path);
        static std::string tempName(const std::string& tempdir = "");

    private:
        bool _keep;
};

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXTEMPORARYFILE_H_
