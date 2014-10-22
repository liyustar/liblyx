#include "lyxSocket.h"
#include <netdb.h>	// For in_port_t
#include <stdio.h>
#include <string.h>

using namespace lyx;

static const in_port_t listen_port = 8888;

int main() {
	TCPSocket clnt("localhost", listen_port);

	char buf[] = "hello world";
	ssize_t ret = clnt.send(buf, sizeof(buf));
	if (ret != sizeof(buf)) {
		perror("send() failed");
	}

	memset(buf, 0, sizeof(buf));

	ret = clnt.recv(buf, sizeof(buf));
	if (ret < 0) {
		perror("recv() failed");
	}

	printf("success, recv: |%s|\n", buf);

	return 0;
}
