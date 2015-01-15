#ifndef LIBLYX_FOUNDATION_LYXUNBUFFEREDSTREAMBUF_H_
#define LIBLYX_FOUNDATION_LYXUNBUFFEREDSTREAMBUF_H_

#include <streambuf>
#include <iosfwd>
#include <ios>

namespace lyx {

template <typename ch, typename tr>
    class BasicUnbufferedStreamBuf: public std::basic_streambuf<ch, tr>
{
    protected:
        typedef std::basic_streambuf<ch, tr> Base;
        typedef std::basic_ios<ch, tr> IOS;
        typedef ch char_type;
        typedef tr char_traits;
        typedef typename Base::int_type int_type;
        typedef typename Base::pos_type pos_type;
        typedef typename Base::off_type off_type;
        typedef typename IOS::openmode openmode;

    public:
        BasicUnbufferedStreamBuf():
            _pb(char_traits::eof()),
            _ispb(false) {
                this->setg(0, 0, 0);
                this->setp(0, 0);
            }

        ~BasicUnbufferedStreamBuf() {
        }

        virtual int_type overflow(int_type c) {
            if (c != char_traits::eof())
                return writeToDevice(char_traits::to_char_type(c));
            else
                return c;
        }

        virtual int_type underflow() {
            if (_ispb) {
                return _pb;
            }
            else {
                int_type c = readFromDevice();
                if (c != char_traits::eof()) {
                    _ispb = true;
                    _pb   = c;
                }
                return c;
            }
        }

        virtual int_type uflow() {
            if (_ispb) {
                _ispb = false;
                return _pb;
            }
            else {
                int_type c = readFromDevice();
                if (c != char_traits::eof()) {
                    _pb = c;
                }
                return c;
            }
        }

        virtual int_type pbackfail(int_type c) {
            if (_ispb) {
                return char_traits::eof();
            }
            else {
                _ispb = true;
                _pb   = c;
                return c;
            }
        }

    protected:
        static int_type charToInt(char_type c) {
            return char_traits::to_int_type(c);
        }

    private:
        virtual int readFromDevice() {
            return char_traits::eof();
        }

        virtual int writeToDevice(char_type) {
            return char_traits::eof();
        }

        int_type _pb;
        bool     _ispb;

        BasicUnbufferedStreamBuf(const BasicUnbufferedStreamBuf&);
        BasicUnbufferedStreamBuf& operator = (const BasicUnbufferedStreamBuf&);
};

typedef BasicUnbufferedStreamBuf<char, std::char_traits<char> > UnbufferedStreamBuf;

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXUNBUFFEREDSTREAMBUF_H_
