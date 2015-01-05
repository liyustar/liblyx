#include "EchoServer.h"
#include "lyxStreamSocket.h"
#include "lyxSocketAddress.h"
#include "lyxTimespan.h"
#include <iostream>

using lyx::Socket;
using lyx::StreamSocket;
using lyx::SocketAddress;

EchoServer::EchoServer():
    _socket(SocketAddress()),
    _thread("EchoServer"),
    _stop(false)
{
    _thread.start(*this);
    _ready.wait();
}

EchoServer::~EchoServer() {
    _stop = true;
    _thread.join();
}

uint16_t EchoServer::port() const {
    return _socket.address().port();
}

void EchoServer::run() {
    _ready.set();
    lyx::Timespan span(250000);
    while (!_stop) {
        if (_socket.poll(span, Socket::SELECT_READ)) {
            StreamSocket ss = _socket.acceptConnection();
            try {
                char buffer[256];
                int n = ss.receiveBytes(buffer, sizeof(buffer));
                while (n > 0 && !_stop) {
                    ss.sendBytes(buffer, n);
                    n = ss.receiveBytes(buffer, sizeof(buffer));
                }
            }
            catch (lyx::Exception& exc) {
                std::cerr << "EchoServer: " << exc.displayText() << std::endl;
            }
        }
    }
}

