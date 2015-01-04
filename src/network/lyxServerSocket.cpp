#include "lyxServerSocket.h"
#include "lyxServerSocketImpl.h"
#include "lyxException.h"

namespace lyx {

ServerSocket::ServerSocket(): Socket(new ServerSocketImpl) {
}

ServerSocket::ServerSocket(const Socket& socket): Socket(socket) {
    if (!dynamic_cast<ServerSocketImpl*>(impl().get()))
        throw InvalidArgumentException("Cannot assign incompatible socket");
}

ServerSocket::ServerSocket(const SocketAddress& address, int backlog): Socket(new ServerSocketImpl) {
    impl()->bind(address, true);
    impl()->listen(backlog);
}

ServerSocket::ServerSocket(uint16_t port, int backlog): Socket(new ServerSocketImpl) {
    IPAddress wildcardAddr;
    SocketAddress address(wildcardAddr, port);
    impl()->bind(address, true);
    impl()->listen(backlog);
}

ServerSocket::~ServerSocket() {
}

ServerSocket& ServerSocket::operator = (const Socket& socket) {
    if (dynamic_cast<ServerSocketImpl*>(socket.impl().get()))
        Socket::operator = (socket);
    else
        throw InvalidArgumentException("Cannot assign incompatible socket");
    return *this;
}

void ServerSocket::bind(const SocketAddress& address, bool reuseAddress) {
    impl()->bind(address, reuseAddress);
}

void ServerSocket::bind(uint16_t port, bool reuseAddress) {
    IPAddress wildcardAddr;
    SocketAddress address(wildcardAddr, port);
    impl()->bind(address, reuseAddress);
}

void ServerSocket::listen(int backlog) {
    impl()->listen(backlog);
}

StreamSocket ServerSocket::acceptConnection(SocketAddress& clientAddr) {
    return StreamSocket(impl()->acceptConnection(clientAddr));
}

StreamSocket ServerSocket::acceptConnection() {
    SocketAddress clientAddr;
    return StreamSocket(impl()->acceptConnection(clientAddr));
}

} // namespace lyx
