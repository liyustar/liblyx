#ifndef LIBLYX_FOUNDATION_LYXMEMORYSTREAM_H_
#define LIBLYX_FOUNDATION_LYXMEMORYSTREAM_H_

#include <streambuf>
#include <iosfwd>
#include <ios>
#include <istream>
#include <ostream>

namespace lyx {

template <typename ch, typename tr>
    class BasicMemoryStreamBuf: public std::basic_streambuf<ch, tr>
{
    protected:
        typedef std::basic_streambuf<ch, tr> Base;
        typedef std::basic_ios<ch, tr> IOS;
        typedef ch char_type;
        typedef tr char_traits;
        typedef typename Base::int_type int_type;
        typedef typename Base::pos_type pos_type;
        typedef typename Base::off_type off_type;

    public:
        BasicMemoryStreamBuf(char_type* pBuffer, std::streamsize bufferSize):
            _pBuffer(pBuffer),
            _bufferSize(bufferSize) {
                this->setg(_Buffer, _pBuffer, _pBuffer + _bufferSize);
                this->setp(_Buffer, _pBuffer + _bufferSize);
            }

        ~BasicMemoryStreamBuf() {
        }

        virtual int_type overflow(int_type /*c*/) {
            return char_traits::eof();
        }

        virtual int_type underflow() {
            return char_traits::eof();
        }

        virtual int sync() {
            return 0;
        }

        std::streamsize charsWritten() const {
            return static_cast<std::streamsize>(this->pptr() - this->pbase());
        }

        void reset() {
            this->setg(_Buffer, _pBuffer, _pBuffer + _bufferSize);
            this->setp(_Buffer, _pBuffer + _bufferSize);
        }

    private:
        char_type*      _pBuffer;
        std::streamsize _bufferSize;

        BasicMemoryStreamBuf();
        BasicMemoryStreamBuf(const BasicMemoryStreamBuf&);
        BasicMemoryStreamBuf& operator = (const BasicMemoryStreamBuf&);
};

typedef BasicMemoryStreamBuf<char, std::char_traits<char> > MemoryStreamBuf;

class MemoryIOS: public virtual std::ios {
    public:
        MemoryIOS(char* pBuffer, std::streamsize bufferSize);
        ~MemoryIOS();

        MemoryStreamBuf* rdbuf();

    protected:
        MemoryStreamBuf _buf;
};

class MemoryInputStream: public MemoryIOS, public std::istream {
    public:
        MemoryInputStream(const char* pBuffer, std::streamsize bufferSize);
        ~MemoryInputStream();
};

class MemoryOutputStream: public MemoryIOS, public std::ostream {
    public:
        MemoryOutputStream(const char* pBuffer, std::streamsize bufferSize);
        ~MemoryOutputStream();
};

inline MemoryStreamBuf* MemoryIOS::rdbuf() {
    return &_buf;
}

inline std::streamsize MemoryOutputStream::charsWritten() const {
    return _buf.charsWritten();
}

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXMEMORYSTREAM_H_
