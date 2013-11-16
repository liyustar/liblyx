#ifndef LYXSOCKET_H
#define LYXSOCKET_H

#include <sys/types.h>	// For data types
#include <sys/socket.h>	// For socket(), connect(), send(), and recv()
#include <netdb.h>		// For gethostbyname(), in_port_t
#include <arpa/inet.h>	// For inet_addr()
#include <unistd.h>		// For close()
#include <netinet/in.h>	// For sockaddr_in
typedef void raw_type;	// Type used for raw data on this platform

#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>

namespace lyx {

	class SocketException: public std::runtime_error {
		public:
			SocketException(const std::string &message) throw();
			SocketException(const std::string &message, const std::string &detail) throw();
	};

	class SocketAddress {
		public:
			enum AddressType { TCP_SOCKET, TCP_SERVER, UDP_SOCKET };

			// Constructor
			SocketAddress(const char *host, const char *service, AddressType atype = TCP_SOCKET)
				throw(SocketException);
			SocketAddress(const char *host, in_port_t port, AddressType aType = TCP_SOCKET)
				throw(SocketException);
			SocketAddress(sockaddr *addrVal = NULL, socklen_t addrLenVal = 0);

			// getter
			std::string getAddress() const throw(SocketException);
			in_port_t getPort() const throw(SocketException);
			sockaddr *getSockaddr() const {
				return (sockaddr *)&addr;
			}
			socklen_t getSockaddrLen() const {
				return addrLen;
			}

			static std::vector<SocketAddress>
				lookupAddresses(const char *host, const char *service,
						AddressType atype = TCP_SOCKET) throw(SocketException);

			static std::vector<SocketAddress>
				lookupAddresses(const char *host, in_port_t port,
						AddressType atype = TCP_SOCKET) throw(SocketException);

		private:
			sockaddr_storage addr;
			socklen_t addrLen;
	};

	class Socket {
		public:
			virtual ~Socket();
			SocketAddress getLocalAddress() throw(SocketException);
			void close();
			static void cleanUp() throw(SocketException);

		private:
			Socket(const Socket &sock);
			void operator=(const Socket &sock);

		protected:
			int sockDesc;
			Socket();
			void createSocket(const SocketAddress &address, int type, int protocol)
				throw(SocketException);
	};

	class CommunicatingSocket: public Socket {
		public:
			void send(const void *buffer, int bufferLen) throw(SocketException);
			size_t recv(void *buffer, int bufferLen) throw(SocketException);
			size_t recvFuuly(void *buffer, int bufferLen) throw(SocketException);
			SocketAddress getForeignAddress() throw(SocketException);
	};

	class TCPSocket: public CommunicatingSocket {
		public:
			TCPSocket(const char *foreignAddress, in_port_t foreignPort) throw(SocketException);
			TCPSocket();
			~TCPSocket();

			void bind(const SocketAddress &localAddress) throw(SocketException);
			void connect(const SocketAddress &foreignAddress) throw(SocketException);
			std::iostream &getStream() throw(SocketException);

		private:
			friend class TCPServerSocket;
			TCPSocket(int sockDesc);

			std::iostream *myStream;
			std::streambuf *myStreambuf;
	};

	class TCPServerSocket: public Socket {
		public:
			TCPServerSocket();
			TCPServerSocket(in_port_t localPort, int queueLen = 5) throw(SocketException);
			void bind(const SocketAddress &localAddress) throw(SocketException);
			TCPSocket *accept() throw(SocketException);

		private:
			void setListen(int queueLen) throw(SocketException);
	};

	class UDPSocket: public Socket {
		public:
			UDPSocket();
			void bind(const SocketAddress &localAddress) throw(SocketException);
			void connect(const SocketAddress &foreignAddress) throw(SocketException);

			void disconnect() throw(SocketException);
			void sendTo(const void *buffer, int bufferLen,
					const SocketAddress, &foreignAddress) throw(SocketException);
			int recvFrom(void *buffer, int bufferLen,
					SocketAddress &sourceAddress) throw(SocketException);
			void setMulticastTTL(unsigned char multicastTTL) throw(SocketException);
			void leaveGroup(const std::string &multicastGroup) throw(SocketException);

		private:
			void setBroadcast();
	};
}

#endif // LYXSOCKET_H
