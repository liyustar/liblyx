#include "lyxNumberFormatter.h"
#include <cstdio>
#include "lyxSocketDefs.h"

namespace lyx {

bool NumberFormatter::append(std::string& str, uint32_t value) {
    char buf[100];
    int sz = std::sprintf(buf, "%u", value);
    str.append(buf, sz);
    return true;
}

std::string NumberFormatter::format(int value) {
    char buf[100];
    int sz = std::sprintf(buf, "%d", value);
    return std::string(buf, sz);
}

} // namespace lyx
