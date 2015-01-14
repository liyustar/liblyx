#include "lyxString.h"
#include "lyxAscii.h"
#include <cctype>
#include <algorithm>

namespace lyx {

std::string toUpper(const std::string& str) {
    std::string result(str);
    toUpperInPlace(result);
    return result;
}

std::string& toUpperInPlace(std::string& str) {
    std::for_each(str.begin(), str.end(), [](char &ch){ ch = std::toupper(ch); });
    return str;
}

std::string toLower(const std::string& str) {
    std::string result(str);
    toLowerInPlace(result);
    return result;
}

std::string& toLowerInPlace(std::string& str) {
    std::for_each(str.begin(), str.end(), [](char &ch){ ch = std::tolower(ch); });
    return str;
}

std::string& trimInPlace(std::string& str) {
    int first = 0;
    int last = int(str.size()) - 1;

    while (first <= last && Ascii::isSpace(str[first])) ++first;
    while (last >= first && Ascii::isSpace(str[last])) --last;

    str.resize(last + 1);
    str.erase(0, first);

    return str;
}

int icompare(const std::string& str1, const std::string& str2) {
    return toLower(str1).compare(toLower(str2));
}

} // namespace lyx
