#ifndef LYXURL_H
#define LYXURL_H

#include <string>

namespace lyx {

	typedef enum {
		PROTO_UNKNOW,
		PROTO_HTTP,
		PROTO_HTTPS,
	}PROTO_TYPE;

	const int PORT_UNKNOW = -1;

	class Url {
		private:
		Url();
		std::string m_url;
		PROTO_TYPE m_proto;
		std::string m_hostname;
		std::string m_path;
		int m_port;

		int urlAnalyse(std::string urlStr);
		// Url(const Url&);
		void print();

		public:
		Url(std::string urlStr);
		// Url(const Url& urlObj);
		~Url();

		// getter
		std::string getUrlStr() const;
		std::string getHostname() const;
		std::string getPath() const;
		int getPort() const;

		void test();
	};
}

#endif // LYXURL_H
