#include "lyxSocket.h"
#include <netdb.h>	// For in_port_t
#include <stdio.h>

using namespace lyx;

static const in_port_t listen_port = 8888;

int main() {
	TCPServerSocket serv(listen_port);
	TCPSocket *conn = serv.accept();
	if (NULL == conn) {
		perror("accept() failed");
	}

	char buf[1024];
	while(1) {
		size_t ret = conn->recv(buf, 1024);
		if (ret < 0) {
			perror("recv() failed");
			break;
		}
		if (ret == 0) {
			printf("client close\n");
			break;
		}
		conn->send(buf, ret);
	}

	delete conn;

	return 0;
}
