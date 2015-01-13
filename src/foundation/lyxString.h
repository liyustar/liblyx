#ifndef LIBLYX_FOUNDATION_LYXSTRING_H_
#define LIBLYX_FOUNDATION_LYXSTRING_H_

#include "lyxAscii.h"
#include <string>

namespace lyx {

inline std::string& trimInPlace(std::string& str) {
    int first = 0;
    int last = int(str.size()) - 1;

    while (first <= last && Ascii::isSpace(str[first])) ++first;
    while (last >= first && Ascii::isSpace(str[last])) --last;

    str.resize(last + 1);
    str.erase(0, first);

    return str;
}

}

#endif // LIBLYX_FOUNDATION_LYXSTRING_H_
