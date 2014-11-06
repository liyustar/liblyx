#ifndef LIBLYX_NETWORK_LYXSTREAMSOCKETIMPL_H_
#define LIBLYX_NETWORK_LYXSTREAMSOCKETIMPL_H_

#include "lyxSocketImpl.h"

namespace lyx {

class StreamSocketImpl : public SocketImpl {
    public:
        StreamSocketImpl();
        explicit StreamSocketImpl(IPAddress::Family addressFamily);
        StreamSocketImpl(int sockfd);

        virtual int sendBytes(const void* buffer, int length, int flags = 0);

    protected:
        virtual ~StreamSocketImpl();
};

} // namespace lyx

#endif // LIBLYX_NETWORK_LYXSTREAMSOCKETIMPL_H_
