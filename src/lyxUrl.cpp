#include "lyxUrl.h"
#include <string>
#include <cstring> // for strlen
#include <cstdlib> // for atoi
#include <iostream>

using namespace std;

namespace lyx {

	Url::Url(string urlStr) {
		m_url = urlStr;
		urlAnalyse(m_url);
	}

//	Url::Url(const Url& urlObj) {
//		*this = urlObj;
//	}

	Url::~Url() {
	}

	/**
	 * parse url
	 * success: 0 		fail: 1
	 */
	int Url::urlAnalyse(string urlStr) {
		unsigned int pos = 0, markpos = 0;
		int parsedone = 0;

		// get protocol
		pos = urlStr.find("://", pos);
		if (string::npos == pos) {
			m_proto = PROTO_UNKNOW;
		} else {
			string protoStr = urlStr.substr(markpos, pos);
			if (protoStr.compare("http") == 0) {
				m_proto = PROTO_HTTP;
			} else if (protoStr.compare("https") == 0) {
				m_proto = PROTO_HTTPS;
			} else {
				m_proto = PROTO_UNKNOW;
			}
			pos += strlen("://");
			markpos = pos;
		}

		// get host
		pos = urlStr.find_first_of("/", markpos);
		string host;
		if (string::npos == pos) {
			host = urlStr.substr(markpos);
			m_path = "/";
			parsedone = 1;
		} else {
			host = urlStr.substr(markpos, pos - markpos);
			markpos = pos;
		}

		// parse host
		string portStr;
		pos = host.find_first_of(":");
		if (string::npos == pos) {
			m_hostname = host;
			m_port = PORT_UNKNOW;
		} else {
			m_hostname = host.substr(0, pos);
			portStr = host.substr(pos + 1);
			m_port = atoi(portStr.c_str());
		}
		if (parsedone == 1) {
			return 0;
		}

		// get path
		pos = urlStr.find_first_of("?", markpos);
		if (string::npos == pos) {
			m_path = urlStr.substr(markpos);
			parsedone = 1;
		} else {
			m_path = urlStr.substr(markpos, pos - markpos);
			pos += strlen("?");
			markpos = pos;
		}
		if (parsedone == 1) {
			return 0;
		}

		// parse query params
		return 0;
	}

	void Url::print() {
		cout << "UrlOBj:" << endl;
		cout << m_url << endl;
		cout << m_proto << endl;
		cout << m_hostname << endl;
		cout << m_port << endl;
		cout << m_path << endl;
		cout << endl;
	}

	void Url::test() {
		print();
	}

	// getter
	string Url::getUrlStr() const {
		return m_url;
	}
	string Url::getHostname() const {
		return m_hostname;
	}
	string Url::getPath() const {
		return m_path;
	}
	int Url::getPort() const {
		return m_port;
	}
}

