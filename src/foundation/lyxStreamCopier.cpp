#include "lyxStreamCopier.h"
#include "lyxBuffer.h"
#include "lyxBugcheck.h"

namespace lyx {

std::streamsize StreamCopier::copyStream(std::istream& istr, std::ostream& ostr, std::size_t bufferSize) {
    lyx_assert (bufferSize > 0);

    Buffer<char> buffer(bufferSize);
    std::streamsize len = 0;
    istr.read(buffer.begin(), bufferSize);
    std::streamsize n = istr.gcount();
    while (n > 0) {
        len += n;
        ostr.write(buffer.begin(), n);
        if (istr && ostr) {
            istr.read(buffer.begin(), bufferSize);
            n = istr.gcount();
        }
        else
            n = 0;
    }
    return len;
}

std::streamsize StreamCopier::copyStreamUnbuffered(std::istream& istr, std::ostream& ostr) {
    char c;
    std::streamsize len = 0;
    istr.get(c);
    while (istr && ostr) {
        ++len;
        ostr.put(c);
        istr.get(c);
    }
    return len;
}

std::streamsize StreamCopier::copyToString(std::istream& istr, std::string& str, std::size_t bufferSize) {
    lyx_assert (bufferSize > 0);

    Buffer<char> buffer(bufferSize);
    std::streamsize len = 0;
    istr.read(buffer.begin(), bufferSize);
    std::streamsize n = istr.gcount();
    while (n > 0) {
        len += n;
        str.append(buffer.begin(), static_cast<std::string::size_type>(n));
        if (istr) {
            istr.read(buffer.begin(), bufferSize);
            n = istr.gcount();
        }
        else
            n = 0;
    }
    return len;
}

} // namespace lyx
