#ifndef LIBLYX_FOUNDATION_LYXCOUNTINGSTREAM_H_
#define LIBLYX_FOUNDATION_LYXCOUNTINGSTREAM_H_

#include "lyxUnbufferedStreamBuf.h"
#include <istream>

namespace lyx {

class CountingStreamBuf: public UnbufferedStreamBuf {
    public:
        CountingStreamBuf();
        ~CountingStreamBuf();

        int chars() const;

    protected:
        // int readFromDevice();
        int writeToDevice(char c);

    private:
        int _chars;
};

class CountingIOS: public virtual std::ios {
    public:
        CountingIOS();
        ~CountingIOS();

        int chars() const;

        CountingStreamBuf* rdbuf();

    protected:
        CountingStreamBuf _buf;
};

class CountingOutputStream: public CountingIOS, public std::ostream {
    public:
        CountingOutputStream();
        ~CountingOutputStream();
};

inline int CountingStreamBuf::chars() const {
    return _chars;
}

inline int CountingIOS::chars() const {
    return _buf.chars();
}

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXCOUNTINGSTREAM_H_
