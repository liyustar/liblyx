#include "lyxStreamSocket.h"

namespace lyx {

StreamSocket::StreamSocket() :
    Socket(new StreamSocketImpl)
{
}

/*
StreamSocket::StreamSocket(const SocketAddress& address) :
    Socket(new StreamSocketImpl(address.family()))
{
}
*/

StreamSocket::StreamSocket(SocketImpl* pImpl) :
    Socket(pImpl)
{
}

StreamSocket::~StreamSocket() {
}

StreamSocket& StreamSocket::operator = (const Socket& socket) {
    if (dynamic_cast<StreamSocketImpl*>(socket.impl().get())) {
        Socket::operator = (socket);
    }
    else {
        // TODO: throw Exception;
    }
    return *this;
}

void StreamSocket::connect(const SocketAddress& address) {
    impl()->connect(address);
}

void StreamSocket::shutdownReceive() {
    impl()->shutdownReceive();
}

void StreamSocket::shutdownSend() {
    impl()->shutdownSend();
}

void StreamSocket::shutdown() {
    impl()->shutdown();
}

int StreamSocket::sendBytes(const void* buffer, int length, int flags) {
    return impl()->sendBytes(buffer, length, flags);
}

int StreamSocket::receiveBytes(void* buffer, int length, int flags) {
    return impl()->receiveBytes(buffer, length, flags);
}

} // namespace lyx
