#ifndef LIBLYX_FOUNDATION_LYXFORMAT_H_
#define LIBLYX_FOUNDATION_LYXFORMAT_H_

#include <string>

namespace lyx {

enum {
    FORMAT_BUFSIZE = 1024
};

std::string format(const char* fmt, ...)
    __attribute__((format(printf, 1, 2)));

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXFORMAT_H_
