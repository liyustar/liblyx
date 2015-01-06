#ifndef LIBLYX_FOUNDATION_LYXNUMBERFORMATTER_H_
#define LIBLYX_FOUNDATION_LYXNUMBERFORMATTER_H_

#include <string>
#include "lyxSocketDefs.h"

namespace lyx {

class NumberFormatter {
    public:
        static bool append(std::string& str, uint32_t value);
        static std::string format(int value);
};

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXNUMBERFORMATTER_H_
