#ifndef LIBLYX_NETWORK_LYXHTTPFIXEDLENGTHSTREAM_H_
#define LIBLYX_NETWORK_LYXHTTPFIXEDLENGTHSTREAM_H_

#include "lyxHTTPBasicStreamBuf.h"
#include <cstddef>
#include <istream>
#include <ostream>

namespace lyx {

class HTTPSession;

class HTTPFixedLengthStreamBuf: public HTTPBasicStreamBuf {
    public:
        typedef HTTPBasicStreamBuf::openmode openmode;
        typedef std::streamsize ContentLength;

        HTTPFixedLengthStreamBuf(HTTPSession& session, ContentLength length, openmode mode);
        ~HTTPFixedLengthStreamBuf();

    protected:
        int readFromDevice(char* buffer, std::streamsize length);
        int writeToDevice(const char* buffer, std::streamsize length);

    private:
        HTTPSession&  _session;
        ContentLength _length;
        ContentLength _count;
};

class HTTPFixedLengthIOS: public virtual std::ios {
    public:
        HTTPFixedLengthIOS(HTTPSession& session, HTTPFixedLengthStreamBuf::ContentLength length, HTTPFixedLengthStreamBuf::openmode mode);
        ~HTTPFixedLengthIOS();
        HTTPFixedLengthStreamBuf* rdbuf();

    protected:
        HTTPFixedLengthStreamBuf _buf;
};

class HTTPFixedLengthInputStream: public HTTPFixedLengthIOS, public std::istream {
    public:
        HTTPFixedLengthInputStream(HTTPSession& session, HTTPFixedLengthStreamBuf::ContentLength length);
        ~HTTPFixedLengthInputStream();

        void* operator new(std::size_t size);
        void operator delete(void* ptr);

    private:
        static lyx::MemoryPool _pool;
};

class HTTPFixedLengthOutputStream: public HTTPFixedLengthIOS, public std::ostream {
    public:
        HTTPFixedLengthOutputStream(HTTPSession& session, HTTPFixedLengthStreamBuf::ContentLength length);
        ~HTTPFixedLengthOutputStream();

        void* operator new(std::size_t size);
        void operator delete(void* ptr);

    private:
        static lyx::MemoryPool _pool;
};

} // namespace lyx

#endif // LIBLYX_NETWORK_LYXHTTPFIXEDLENGTHSTREAM_H_
