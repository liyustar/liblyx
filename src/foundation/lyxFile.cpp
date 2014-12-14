#include "lyxFile.h"

namespace lyx {

void File::handleLastError(const std::string& path) {
    handleLastErrorImpl(path);
}

} // namespace lyx
