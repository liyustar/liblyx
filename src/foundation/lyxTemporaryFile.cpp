#include "lyxTemporaryFile.h"
#include "lyxPath.h"
#include "lyxMutex.h"
#include "lyxException.h"
#include "lyxBugcheck.h"
#include <set>
#include <sstream>

namespace lyx {

class TempFileCollector {
    public:
        TempFileCollector() {
        }

        ~TempFileCollector() {
            try {
                for (std::set<std::string>::iterator it = _files.begin(); it != _files.end(); ++it) {
                    try {
                        File f(*it);
                        if (f.exists())
                            f.remove(true);
                    }
                    catch (Exception&) {
                    }
                }
            }
            catch (...) {
                lyx_unexpected();
            }
        }

        void registerFile(const std::string& path) {
            FastMutex::ScopedLock lock(_mutex);

            Path p(path);
            _files.insert(p.absolute().toString());
        }

    private:
        std::set<std::string> _files;
        FastMutex _mutex;
};

TemporaryFile::TemporaryFile():
    File(tempName()),
    _keep(false)
{
}

TemporaryFile::TemporaryFile(const std::string& tempDir):
    File(tempName(tempDir)),
    _keep(false)
{
}

TemporaryFile::~TemporaryFile() {
    try {
        if (!_keep) {
            try {
                if (exists())
                    remove(true);
            }
            catch (Exception&) {
            }
        }
    }
    catch (...) {
        lyx_unexpected();
    }
}

void TemporaryFile::keep() {
    _keep = true;
}

void TemporaryFile::keepUntilExit() {
    _keep = true;
    registerForDeletion(path());
}

namespace {
static TempFileCollector fc;
}

void TemporaryFile::registerForDeletion(const std::string& path) {
    fc.registerFile(path);
}

namespace {
static FastMutex mutex;
}

std::string TemporaryFile::tempName(const std::string& tempDir) {
    std::ostringstream name;
    static unsigned long count = 0;
    mutex.lock();
    unsigned long n = count++;
    mutex.unlock();
    name << (tempDir.empty() ? Path::temp() : tempDir);
    if (name.str().at(name.str().size() - 1) != Path::separator()) {
        name << Path::separator();
    }

    name << "tmp";

    for (int i = 0; i < 6; ++i) {
        name << char('a' + (n % 26));
        n /= 26;
    }
    return name.str();
}

} // namespace lyx
