#include "lyxSocket.h"

#include <errno.h>		// For errno
using namespace std;

namespace lyx {

	/**
	 * SocketException
	 */
	SocketException::SocketException(const string &message) throw()
		: runtime_error(message) {
		}

	SocketException::SocketException(const string &message, const string &detail) throw()
		: runtime_error(message + ": " + detail) {
		}

	/**
	 * SocketAddress
	 */
	static addrinfo *getAddressInfo(const char *host, const char *service,
			SocketAddress::AddressType atype) throw(SocketException) {
		// Create criteria for the socket we require
		addrinfo addrCriteria;
		memset(&addrCriteria, 0, sizeof(addrCriteria));	// Zero out structure
		addrCriteria.ai_family = AF_UNSPEC;				// Any address family

		switch (atype) {
			case SocketAddress::TCP_SOCKET:
				// Ask for addresses for a TCP socket
				addrCriteria.ai_socktype = SOCK_STREAM;
				addrCriteria.ai_protocol = IPPROTO_TCP;
				break;
			case SocketAddress::TCP_SERVER:
				// Ask for addresses for a TCP server socket
				addrCriteria.ai_socktype = SOCK_STREAM;
				addrCriteria.ai_protocol = IPPROTO_TCP;
				addrCriteria.ai_flags = AI_PASSIVE;
				break;
			case SocketAddress::UDP_SOCKET:
				// Ask for addresses for a UDPsocket
				addrCriteria.ai_socktype = SOCK_DGRAM;
				addrCriteria.ai_protocol = IPPROTO_UDP;
				break;
		}

		// Get linked list of remote addresses
		addrinfo *servAddrs;
		int rtnVal = getaddrinfo(host, service, &addrCriteria, &servAddrs);
		if (rtnVal != 0)
			throw SocketException("getaddrinfo() failed", gai_strerror(rtnVal));

		return servAddrs;
	}

	SocketAddress::SocketAddress(const char *host, const char *service,
			AddressType atype) throw(SocketException) {
		addrinfo *servAddrs = getAddressInfo(host, service, atype);

		// Extract just the first address
		if (servAddrs == NULL)
			throw SocketException("No matching socket address");

		addrLen = servAddrs->ai_addrLen;
		memcpy( &addr, servAddrs->ai_addr, addrLen );

		freeaddrinfo(servAddrs);
	}

	SocketAddress::SocketAddress(const char *host, in_port_t port,
			AddressType atype) throw(SocketException) {
		// Convert the numeric port request into a string.
		char service[6];
		snprintf(service, sizeof(service), "%d", port);
		addrinfo *servAddrs = getAddressInfo(host, service, atype);

		// Extract just the first address
		if (servAddrs == NULL)
			throw SocketException("No matching socket address");

		addrLen = servAddrs->ai_addrLen;
		memcpy( &addr, servAddrs->ai_addr, addrLen );

		freeaddrinfo(servAddrs);
	}

	SocketAddress::SocketAddress(sockaddr *addrVal, socklen_t addrLenVal) {
		addrLen = addrLenVal;
		memcpy( &addr, addrVal, addrLen);
	}

	string SocketAddress::getAddress() const throw(SocketException) {
		void *numericAddress;

		switch (((sockaddr *)&addr)->sa_family) {
			case AF_INET:
				numericAddress = &(((sockaddr_in *)&addr)->sin_addr.s_addr);
				break;
			case AF_INET6:
				numericAddress = &(((sockaddr_in6 *)&addr)->sin6_addr.s6_addr);
				break;
			default:
				throw SocketException("Unknown address type");
		}

		static char addrBuffer[INET6_ADDRSTRLEN];
		if (inet_ntop(((sockaddr *)&addr)->sa_family, numericAddress, addrBuffer,
					sizeof(addrBuffer)) == NULL) {
			throw SocketException("Unable to get address");
		}

		return addrBuffer;
	}

	in_port_t SocketAddress::getPort() const throw(SocketException) {
		switch (((sockaddr *)&addr)->sa_family) {
			case AF_INET:
				return ntohs(((sockaddr_in *)&addr)->sin_port);
			case AF_INET6:
				return ntohs(((sockaddr_in6 *)&addr)->sin6_port);
			default:
				throw SocketException("Unknown address type");
		}
	}

	vector<SocketAddress> ScoketAddress::lookupAddresses(const char *host,
			const char *service, AddressType atype) throw(SocketException) {
		addrinfo *servAddrs = getAddressInfo(host, service, atype);

		// Push a copy of each address onto our list
		vector<SocketAddress> addrList;
		for (addrinfo *curAddr = servAddrs; curAddr != NULL;
				curAddr = curAddr->ai_next)
			addrList.push_back(SocketAddress((sockaddr *)(curAddr->ai_addr),
						curAddr->ai_addrLen));

		freeaddrinfo(servAddrs);

		return addrList;
	}

	vector<SocketAddress> ScoketAddress::lookupAddresses(const char *host,
			in_port_t port, AddressType atype) throw(SocketException) {
		// Convert the numeric port request into a string.
		char service[6];
		snprintf(service, sizeof(service), "%d", port);
		addrinfo *servAddrs = getAddressInfo(host, service, atype);

		// Push a copy of each address onto our list
		vector<SocketAddress> addrList;
		for (addrinfo *curAddr = servAddrs; curAddr != NULL;
				curAddr = curAddr->ai_next)
			addrList.push_back(SocketAddress((sockaddr *)(curAddr->ai_addr),
						curAddr->ai_addrLen));

		freeaddrinfo(servAddrs);

		return addrList;
	}

	/**
	 * Socket
	 */
	Socket::Socket() {
		// No socket descriptor defined yet
		sockDesc = -1;
	}

	Scoket::~Socket() {
		if (sockDesc >= 0) {
			close();
		}
	}

	void Socket::cleanUp() throw(SocketExcption) {
		// For Windows Socket
	}

	SocketAddress Socket::getLocalAddress() throw(SocketException) {
		sockaddr_storage addr;
		socklen_t addrLen = sizeof(addr);

		if (getsockname(sockDesc, (sockaddr*) &addr, &addrLen) < 0) {
			throw SocketException("Fetch of local address failed (getssockname())");
		}

		return SocketAddress((sockaddr *)&addr, addrLen);
	}

	void Socket::close() {
		shutdown(sockDesc, SHUT_RD);
		::close(sockDesc);
		sockDesc = -1;
	}

	void Socket::createSocket(const SocketAddress &address, int type,
			int protocol) throw(SocketException) {
		// Destroy the old socket if there was one
		if (sockDesc >= 0)
			close();

		int domain = PF_INET;

		// Look at the address and see what domain we should ask for
		if (address.getSockaddr()->sa_family == AF_INET6)
			domain = PF_INET6;

		sockDesc = socket(domain, type, protocol);
		if (sockDesc < 0)
			throw SocketException(string("Can't create socket : ") + strerror(errno));
	}

	/**
	 * CommunicatingSocket
	 */
	void CommunicatingSocket::send(const void *buffer, int bufferLen)
		throw(SocketException) {
		if (::send(sockDesc, (raw_type *)buffer, bufferLen, 0) < 0) {
			throw SocketException("Send failed (send())");
		}
	}

	size_t CommunicatingSocket::recv(void *buffer, int bufferLen)
		throw(SocketException) {
		int rtn = ::recv(sockDesc, (raw_type *)buffer, bufferLen, 0);
		if (rtn < 0) {
			throw SocketException("Receive failed (recv())");
		}

		return rtn;
	}

	size_t CommunicatingSocket::recvFully(void *buffer, int bufferLen)
		throw(SocketException) {
		int rcount = 0;
		int len = ::recv(sockDesc, (raw_type *)buffer, bufferLen, 0);
		while (len > 0 && rcount + len < bufferLen) {
			rcount += len;
			len = ::recv(sockDesc, (raw_type *)(((char *) buffer) + rcount),
					bufferLen - rcount, 0);
		}

		if (len < 0) {
			throw SocketException("Receive failed (recv())");
		}

		return rcount + len;
	}

	SocketAddress CommunicatingSocket::getForeignAddress() throw(SocketException) {
		sockaddr_storage addr;
		socklen_t addrLen = sizeof(addr);

		if (getpeername(sockDesc, (sockaddr *) &addr, &addrLen) < 0) {
			throw SocketException("Fetch of foreign address failed (getpeername())");
		}

		return SocketAddress((sockaddr *)&addr, addrLen);
	}


} // namespace lyx
