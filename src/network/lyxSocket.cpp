#include "lyxSocket.h"
#include "lyxStreamSocketImpl.h"

namespace lyx {

/*
Socket::Socket():
    _pImpl(new StreamSocketImpl)
{
}
*/

Socket::Socket(Ptr pImpl):
    _pImpl(pImpl)
{
}

Socket::Socket(const Socket& socket):
    _pImpl(socket._pImpl)
{
}

Socket::~Socket() {
}

} // namespace lyx
