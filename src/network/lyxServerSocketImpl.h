#ifndef LIBLYX_NETWORK_LYXSERVERSOCKETIMPL_H_
#define LIBLYX_NETWORK_LYXSERVERSOCKETIMPL_H_

#include "lyxSocketImpl.h"

namespace lyx {

class ServerSocketImpl : public SocketImpl {
    public:
        ServerSocketImpl();

    protected:
        virtual ~ServerSocketImpl();
};

} // namespace lyx
#endif // LIBLYX_NETWORK_LYXSERVERSOCKETIMPL_H_
