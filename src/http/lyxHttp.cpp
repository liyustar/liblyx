#include "lyxHttp.h"
#include "lyxUrl.h"
#include "lyxSocket.h"
#include "lyxCookieStorage.h"
#include <unistd.h>
#include <iostream>
#include <string>
#include <map>
#include <utility>
#include <cstdio>
#include <cstdlib>

using namespace std;


static string myitoa(int n) {
	const int INTBUFSIZE = 12;
	char buf[INTBUFSIZE] = {0};
	sprintf(buf, "%d", n);
	string num = buf;
	return num;
}

namespace lyx {

	map<string, Http::TOKEN_TYPE> Http::headerTokenMap;
	map<Http::TOKEN_TYPE, Http::TOKEN_METHOD> Http::tokenMethodMap;

	static Http::HeaderProcess headerProc[] = {
		{Http::TOKEN_CONTENT_LENGTH,	"Content-Length",	Http::parseContentLength},
		{Http::TOKEN_SET_COOKIE,		"Set-Cookie",		Http::parseSetCookie},
		{Http::TOKEN_UNKNOW, "", NULL }
	};

	Http::Http() : m_url("") { }

	Http::Http(Url url) : m_url(url) { }

	Http::~Http() {}

	int Http::initHttpAlgorithms() {
		static int hasInit = 0;
		if (hasInit == 1) return 0;
		hasInit = 1;
		loadHeaderTokenMap();
		return 0;
	}

	int Http::setMethod(string methodStr) {
		if (methodStr.compare("GET") == 0
			|| methodStr.compare("get") == 0) {
			m_method = METHOD_GET;
		} else if (methodStr.compare("POST") == 0
					|| methodStr.compare("post") == 0) {
			m_method = METHOD_POST;
		} else {
			m_method = METHOD_UNKNOW;
		}
		return 0;
	}

	int Http::addParam(const string key, const string val) {
		// TODO: process '&' '?' etc.
		m_params.push_back(make_pair(key, val));
		return 0;
	}

	int Http::addHeader(const string title, const string content) {
		// TODO: process '&' '?' etc.
		m_headers.push_back(make_pair(title, content));
		return 0;
	}

	string Http::getMethodStr() const {
		string methodStr;
		switch (m_method) {
			case METHOD_POST:
				methodStr = "POST";
				break;
			case METHOD_GET: /* no break */
			default:
				methodStr = "GET";
				break;
		}
		return methodStr;
	}

	string Http::getParamsStr() const {
		string paramsStr;
		int len = m_params.size();
		for (int i = 0; i < len; i++) {
			const Param &param = m_params[i];
			paramsStr.append(param.first);
			paramsStr.append("=");
			paramsStr.append(param.second);
			if (i < len - 1) {
				paramsStr.append("&");
			}
		}
		return paramsStr;
	}

	string Http::getHeadersStr() const {
		string headersStr;
		int len = m_headers.size();
		for (int i = 0; i < len; i++) {
			const Header &header = m_headers[i];
			headersStr.append(header.first);
			headersStr.append(": ");
			headersStr.append(header.second);
			headersStr.append("\r\n");
		}
		return headersStr;
	}

	string Http::getCookiesStr() const {
		CookieStorageInstence csInstence = CookieStorage::getCookieStorageInstence();
		CookieSet cookieSet = csInstence->getCookiesByUrl(m_url);
		CookieSetIter iter = cookieSet.begin();
		string cookies;
		for ( ; iter != cookieSet.end(); iter++) {
			cookies.append(" ").append(iter->getName());
			cookies.append("=").append(iter->getVal()).append(";");
		}
		if (cookies.empty())
			return cookies;
		cookies.insert(0, "Cookie:");
		cookies.append("\r\n");
		return cookies;
	}

	int Http::createRequest(std::string &request) {
		string body, path;
		string cookies = getCookiesStr();
		string headers = getHeadersStr();
		string params = getParamsStr();
		if (m_method == METHOD_GET) {
			path = m_url.getPath();
			if (!params.empty())
				path.append("?").append(params);
		}
		else {
			path = m_url.getPath();
			if (!params.empty())
				body = params;
		}
		request.clear();
		request.append(getMethodStr()).append(" ");
		request.append(path).append(" HTTP/1.1\r\n");
		request.append("Host: ").append(m_url.getHostname()).append("\r\n");

		if (!cookies.empty())
			request.append(cookies);

		if (!body.empty()) {
			request.append("Content-Length: ")
				.append(myitoa(body.size())).append("\r\n");
		}

		if (!headers.empty())
			request.append(headers);

		request.append("Connection: Close\r\n");	// not support keep-alive
		request.append("\r\n");
		if (!body.empty()) {
			request.append(body);
		}
		return 0;
	}

	int Http::sendRequest(TCPSocket *psock, const string &request) {
		psock->send(request.c_str(), request.size());
		return 0;
	}

	int Http::recvResponse(TCPSocket *psock, string &header, string &response) {
		int isFindHeader = false;
		const int BUFLEN = 1024 * 8;
		char buf[BUFLEN];
		int totalrecv = 0;
		do {
			int len = psock->recv(buf, BUFLEN);
			if (len > 0) {
				response.append(buf, len);
				// find header
				if (isFindHeader == false
					&& response.find("\r\n\r\n") != string::npos) {
					isFindHeader = true;
					int pos = response.find("\r\n\r\n");
					header = response.substr(0, pos + 4);
					response = response.substr(pos + 4);
					// cout << header;
				}
			} else if (len == 0) {
				break;
			} else {
				perror("");
				break;
			}
			totalrecv += len;
		} while(true);
		// cout << "recv len: " << totalrecv << endl;
		return 0;
	}

	int Http::processHeaderLine(const string &tokStr, const string &content) {
		TOKEN_TYPE token;
		TOKEN_METHOD method;
		map<string, TOKEN_TYPE>::iterator tokIter = headerTokenMap.find(tokStr);
		if (tokIter == headerTokenMap.end()) {
			return -1;
		}
		token = tokIter->second;
		map<TOKEN_TYPE, TOKEN_METHOD>::iterator methIter = tokenMethodMap.find(token);
		if (methIter == tokenMethodMap.end()) {
			return -1;
		}
		method = methIter->second;
		return method(this, content);
	}

	int Http::analyseHeaderFirstLine(const string &firstLine) {
		int pos = 0, end = 0, status = 0;
		string token;
		end = firstLine.find_first_not_of(" ", pos);
		token = firstLine.substr(pos, end - pos); // HTTP version
		pos = end + 1;
		end = firstLine.find_first_not_of(" ", pos);
		token = firstLine.substr(pos, end - pos); // response code
		status = atoi(token.c_str());
		pos = end + 1;
		end = firstLine.find_first_not_of("\r\n", pos);
		token = firstLine.substr(pos, end - pos); // response discription
		return status;
	}

	int Http::analyseHeaderLine(const string &line) {
		unsigned int pos = 0, end = 0;
		string token, content;
		end = line.find(":");
		if (string::npos == end) {
			return -1;
		}
		token = line.substr(pos, end - pos);
		// skip ' '
		pos = end;
		pos = line.find_first_not_of(" :", pos);
		end = 1 + line.find_last_not_of(" \r\n", line.size());
		content = line.substr(pos, end - pos);
		processHeaderLine(token, content);
		return 0;
	}

	int Http::analyzeResponseHeader(const string &header, int &status) {
		string curLine;
		for (unsigned int pos = 0, end = 0;
				(end = header.find("\r\n", pos));
				pos = end + 2) {
			if (string::npos == end) {
				perror("error: header have no \\r\\n\n");
				return -1;
			}
			curLine = header.substr(pos, end - pos);
			// if (curLine.compare("\r\n") == 0) {
			if (end == pos) {
				break;
			}
			if (pos == 0) {
				// first line
				// eg: HTTP/1.1 200 OK
				status = analyseHeaderFirstLine(curLine);
			}
			else {
				// other line
				// eg: Content-Line: 56\r\n
				analyseHeaderLine(curLine);
			}
		}
		return 0;
	}

	int Http::getResponse(string &response) {
		// create http request
		string request;
		string header;
		int status = 0;
		int res = 0; // result
		TCPSocket *sock = NULL;
		if (m_url.getPort() != 443) {
			sock = new TCPSocket(m_url.getHostname().c_str(), m_url.getPort());
		} else {
			sock = new TCPSslSocket(m_url.getHostname().c_str(), m_url.getPort());
		}

		res = createRequest(request);
		res = sendRequest(sock, request);
		res = recvResponse(sock, header, response);
		res = analyzeResponseHeader(header, status);
		cout << request << endl;
		cout << header << endl;
		test();
		delete sock;
		return res;
	}

	// init function
	void Http::loadHeaderTokenMap() {
		for (int i = 0; headerProc[i].tok != TOKEN_UNKNOW; i++) {
			headerTokenMap.insert(make_pair(headerProc[i].tokStr,
											headerProc[i].tok));
			tokenMethodMap.insert(make_pair(headerProc[i].tok,
											headerProc[i].tokMethod));
		}
	}

	// parse header line
	int Http::parseContentLength(HttpCtx ctx, string str) {
		// TODO
		int len = atoi(str.c_str());
		cout << "Content-Length: " << len << endl;
		return 0;
	}

	int Http::parseSetCookie(HttpCtx ctx, string setCookiestr) {
		// TODO
		CookieStorageInstence csInstence = CookieStorage::getCookieStorageInstence();
		Cookie cookie = Cookie::parseSetCookieString(ctx->getUrl(), setCookiestr);
		csInstence->addCookie(cookie);
		return 0;
	}

	Url Http::getUrl() const {
		return m_url;
	}

	void Http::test() {
		CookieStorageInstence csInstence = CookieStorage::getCookieStorageInstence();
		CookieSet cookies = csInstence->getCookiesByUrl(m_url);
		CookieSetIter iter = cookies.begin();
		for ( ; iter != cookies.end(); iter++) {
			cout << iter->getName() << "=" << iter->getVal() << endl;
		}
		cout << endl;
	}
}

