#ifndef LIBLYX_FOUNDATION_LYXSTRING_H_
#define LIBLYX_FOUNDATION_LYXSTRING_H_

#include <string>

namespace lyx {

std::string  toUpper(const std::string& str);
std::string& toUpperInPlace(std::string& str);

std::string  toLower(const std::string& str);
std::string& toLowerInPlace(std::string& str);

std::string& trimInPlace(std::string& str);

int icompare(const std::string& str1, const std::string& str2);

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXSTRING_H_
