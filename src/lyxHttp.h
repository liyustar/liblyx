// !!! need call Http::initHttpAlgorithms();
//
#ifndef LYXHTTP_H
#define LYXHTTP_H

#include "lyxUrl.h"
#include "lyxSocket.h"
#include <string>
#include <vector>
#include <map>

namespace lyx {

	typedef class Http * HttpCtx;

	class Http {

		public:
			typedef enum {
				METHOD_UNKNOW,
				METHOD_GET,
				METHOD_POST,
			}METHOD_TYPE;

			typedef enum {
				TOKEN_UNKNOW,
				TOKEN_CONTENT_LENGTH,
				TOKEN_SET_COOKIE,
			}TOKEN_TYPE;

			typedef int (*TOKEN_METHOD)(HttpCtx, std::string);

			typedef struct {
				TOKEN_TYPE tok;
				std::string tokStr;
				TOKEN_METHOD tokMethod;
			}HeaderProcess;

			typedef std::pair<std::string, std::string> Param;
			typedef std::pair<std::string, std::string> Header;

		public:
			Http();
			Http(Url url);
			// Http(const Http& httpObj);
			~Http();

			static int initHttpAlgorithms();

			int setMethod(std::string method);
			int addParam(const std::string key, const std::string val);
			int addHeader(const std::string title, const std::string content);

			// return status
			int getResponse(std::string &response);

			void test();

			// parse header line
			static int parseContentLength(HttpCtx, std::string);
			static int parseSetCookie(HttpCtx, std::string);

			// getter
			Url getUrl() const;

		private:
			Url m_url;
			METHOD_TYPE m_method;
			std::vector<Param> m_params;
			std::vector<Header> m_headers;
			static std::map<std::string, TOKEN_TYPE> headerTokenMap;
			static std::map<TOKEN_TYPE, TOKEN_METHOD> tokenMethodMap;

			// Http(const Http&);
			void print();

			std::string getMethodStr() const;
			std::string getParamsStr() const;
			std::string getHeadersStr() const;
			std::string getCookiesStr() const;
			int createRequest(std::string &request);
			int sendRequest(Socket *psock, const std::string &request);
			int recvResponse(Socket *psock, std::string &header, std::string &response);
			int analyzeResponseHeader(const std::string &header, int &status);
			int processHeaderLine(const std::string &token, const std::string &content);
			int analyseHeaderFirstLine(const std::string &firstLine);
			int analyseHeaderLine(const std::string &line);

			// init function
			static void loadHeaderTokenMap();
	};
}

#endif // LYXHTTP_H
