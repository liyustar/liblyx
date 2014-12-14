#ifndef LIBLYX_FOUNDATION_LYXDIGESTSTREAM_H_
#define LIBLYX_FOUNDATION_LYXDIGESTSTREAM_H_

#include "lyxBufferedStreamBuf.h"
#include "DigestEngine.h"
#include <istream>
#include <ostream>

namespace lyx {

class DigestBuf: public BufferedStreamBuf {
    public:
        DigestBuf(DigestEngine& eng);
        DigestBuf(DigestEngine& eng, std::istream& istr);
        DigestBuf(DigestEngine& eng, std::ostream& ostr);
        ~DigestBuf();

        int readFromDevice(char* buffer, std::streamsize length);
        int writeToDevice(const char* buffer, std::streamsize length);
        void close();

    private:
        DigestEngine& _eng;
        std::istream* _pIstr;
        std::ostream* _pOstr;
        static const int BUFFER_SIZE;
};

class DigestIOS: public virtual std::ios {
    public:
        DigestIOS(DigestEngine& eng);
        DigestIOS(DigestEngine& eng, std::istream& istr);
        DigestIOS(DigestEngine& eng, std::ostream& ostr);
        ~DigestIOS();
        DigestBuf* rdbuf();

    protected:
        DigestBuf _buf;
};

class DigestInputStream:public DigestIOS, public std::istream {
    public:
        DigestInputStream(DigestEngine& eng, std::istream& istr);
        ~DigestInputStream();
};

class DigestOutputStream:public DigestIOS, public std::ostream {
    public:
        DigestOutputStream(DigestEngine& eng);
        DigestOutputStream(DigestEngine& eng, std::ostream& ostr);
        ~DigestOutputStream();
        void close();
};

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXDIGESTSTREAM_H_
