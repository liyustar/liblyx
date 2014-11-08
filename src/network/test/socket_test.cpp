#include "lyxSocketImpl.h"
#include "lyxSocket.h"
#include "gtest/gtest.h"
#include <string>

using namespace lyx;

TEST(SocketImplTest, connect) {
    SocketImpl sock;
    std::string request = "GET / HTTP/1.0\r\n\r\n";
    char buf[1024];

    sock.connect(SocketAddress("localhost", 80));

    sock.sendBytes(request.c_str(), request.length());
    int len = sock.receiveBytes(buf, 1024);
    std::string response(buf, len);
    int pos = response.find("\r\n\r\n") + 4;

    EXPECT_EQ("hello world.\n", response.substr(pos));
}

TEST(SocketTest, connect) {
    Socket sock;
    std::string request = "GET / HTTP/1.0\r\n\r\n";
    char buf[1024];

    sock.connect(SocketAddress("localhost", 80));

    sock.sendBytes(request.c_str(), request.length());
    int len = sock.receiveBytes(buf, 1024);
    std::string response(buf, len);
    int pos = response.find("\r\n\r\n") + 4;

    EXPECT_EQ("hello world.\n", response.substr(pos));
}
