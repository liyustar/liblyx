#include "lyxSocketImpl.h"
#include "lyxStreamSocket.h"
#include "lyxException.h"
#include "lyxTimespan.h"
#include "lyxStopwatch.h"
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

TEST(SocketImplTest, Exception) {
    SocketImpl sock;
    std::string request = "GET / HTTP/1.0\r\n\r\n";
    try {
        sock.sendBytes(request.c_str(), request.length());
        FAIL();
    } catch(Exception &e) {
        EXPECT_EQ("Invalid socket", e.displayText());
    }
}

TEST(StreamSocketTest, connect) {
    StreamSocket sock;
    std::string request = "GET / HTTP/1.0\r\n\r\n";
    char buf[1024];

    sock.connect(SocketAddress("localhost", 80));

    sock.sendBytes(request.c_str(), request.length());
    int len = sock.receiveBytes(buf, 1024);
    std::string response(buf, len);
    int pos = response.find("\r\n\r\n") + 4;

    EXPECT_EQ("hello world.\n", response.substr(pos));
}

TEST(StreamSocketTest, Echo) {
    StreamSocket sock;
    sock.connect(SocketAddress("localhost", "echo"));
    int n = sock.sendBytes("hello", 5);
    EXPECT_EQ(5, n);

    char buf[1024];
    n = sock.receiveBytes(buf, sizeof(buf));
    EXPECT_EQ(5, n);
    EXPECT_EQ("hello", std::string(buf, n));
    sock.close();
}

TEST(SocketTest, testTimeout) {
    StreamSocket ss;
    ss.connect(SocketAddress("localhost", "echo"));

    Timespan timeout0 = ss.getReceiveTimeout();
    Timespan timeout(250000);
    ss.setReceiveTimeout(timeout);
    Timespan timeout1 = ss.getReceiveTimeout();

    std::cout << "original receive timeout:  " << timeout0.totalMicroseconds() << std::endl;
    std::cout << "requested receive timeout: " << timeout.totalMicroseconds() << std::endl;
    std::cout << "actual receive timeout:    " << timeout1.totalMicroseconds() << std::endl;

    Stopwatch sw;
    try {
        char buffer[256];
        sw.start();
        ss.receiveBytes(buffer, sizeof(buffer));
        FAIL();
    }
    catch (TimeoutException&) {
    }
    EXPECT_TRUE(sw.elapsed() < 1000000);

    timeout0 = ss.getSendTimeout();
    ss.setSendTimeout(timeout);
    timeout1 = ss.getSendTimeout();

    std::cout << "original send timeout:  " << timeout0.totalMicroseconds() << std::endl;
    std::cout << "requested send timeout: " << timeout.totalMicroseconds() << std::endl;
    std::cout << "actual send timeout:    " << timeout1.totalMicroseconds() << std::endl;
}
