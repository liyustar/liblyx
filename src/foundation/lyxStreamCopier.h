#ifndef LIBLYX_FOUNDATION_LYXSTREAMCOPIER_H_
#define LIBLYX_FOUNDATION_LYXSTREAMCOPIER_H_

#include <string>
#include <istream>
#include <ostream>
#include <cstddef>

namespace lyx {

class StreamCopier {
    public:
        static std::streamsize copyStream(std::istream& istr, std::ostream& ostr, std::size_t bufferSize = 8192);

        static std::streamsize copyStreamUnbuffered(std::istream& istr, std::ostream& ostr);

        static std::streamsize copyToString(std::istream& istr, std::string& str, std::size_t bufferSize = 8192);

};

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXSTREAMCOPIER_H_
