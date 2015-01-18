#ifndef LIBLYX_NETWORK_LYXHTTPHEADERSTREAM_H_
#define LIBLYX_NETWORK_LYXHTTPHEADERSTREAM_H_

#include "lyxHTTPBasicStreamBuf.h"
#include "lyxMemoryPool.h"
#include <cstddef>
#include <istream>
#include <ostream>

namespace lyx {

class HTTPSession;

class HTTPHeaderStreamBuf: public HTTPBasicStreamBuf {
    public:
        typedef HTTPBasicStreamBuf::openmode openmode;

        HTTPHeaderStreamBuf(HTTPSession& session, openmode mode);
        ~HTTPHeaderStreamBuf();

    protected:
        int readFromDevice(char* buffer, std::streamsize length);
        int writeToDevice(const char* buffer, std::streamsize length);

    private:
        HTTPSession& _session;
        bool         _end;
};

class HTTPHeaderIOS: public virtual std::ios {
    public:
        HTTPHeaderIOS(HTTPSession& session, HTTPHeaderStreamBuf::openmode mode);
        ~HTTPHeaderIOS();
        HTTPHeaderStreamBuf* rdbuf();

    protected:
        HTTPHeaderStreamBuf _buf;
};

class HTTPHeaderInputStream: public HTTPHeaderIOS, public std::istream {
    public:
        HTTPHeaderInputStream(HTTPSession& session);
        ~HTTPHeaderInputStream();

        void* operator new(std::size_t size);
        void operator delete(void* ptr);

    private:
        static MemoryPool _pool;
};

class HTTPHeaderOutputStream: public HTTPHeaderIOS, public std::ostream {
    public:
        HTTPHeaderOutputStream(HTTPSession& session);
        ~HTTPHeaderOutputStream();

        void* operator new(std::size_t size);
        void operator delete(void* ptr);

    private:
        static MemoryPool _pool;
};

} // namespace lyx

#endif // LIBLYX_NETWORK_LYXHTTPHEADERSTREAM_H_
