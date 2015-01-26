#include "lyxFormat.h"

#include <cstdarg>

namespace lyx {

std::string format(const char* fmt, ...) {
    char buf[FORMAT_BUFSIZE];

    va_list args;
    va_start(args, fmt);
    int num = vsnprintf(buf, FORMAT_BUFSIZE, fmt, args);
    va_end(args);

    if (num < 0)
        return std::string();
    else
        return std::string(buf, num);
}

} // namespace lyx
