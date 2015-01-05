#ifndef ECHOSERVER_H_
#define ECHOSERVER_H_

#include "lyxServerSocket.h"
#include "lyxThread.h"
#include "lyxEvent.h"

class EchoServer: public lyx::Runnable {
    public:
        EchoServer();
        ~EchoServer();
        uint16_t port() const;
        void run();

    private:
        lyx::ServerSocket _socket;
        lyx::Thread       _thread;
        lyx::Event        _ready;
        bool              _stop;
};

#endif // ECHOSERVER_H_
