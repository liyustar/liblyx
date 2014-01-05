/*************************************************************************
    > File Name: lyxCookie.h
    > Author: liyuxing
    > Mail: liyustar@gmail.com 
    > Created Time: 2013年10月11日 星期五 13时29分25秒
 ************************************************************************/

#ifndef LYXCOOKIE_H
#define LYXCOOKIE_H

#include "lyxUrl.h"
#include <string>

namespace lyx {

	class Cookie {
		public:
			Cookie(std::string name, std::string val, std::string domain,
					std::string path = "/", int secure = 0);
			// Cookie(const Cookie& cookieObj);
			~Cookie();

			static Cookie parseSetCookieString(Url url, const std::string &setCookieStr);

			// getter
			std::string getName() const;
			std::string getVal() const;
			std::string getCookieStr() const; // name=val
			std::string getPath() const;
			std::string getDomain() const;
			int getSecure() const;

			bool operator < (const Cookie &cookie) const;
			bool operator == (const Cookie &cookie) const;

			void test();

		private:
			Cookie();
			std::string m_name;
			std::string m_val;
			std::string m_domain;
			std::string m_path;
			int m_secure;

			// Cookie(const Cookie&);
			void print();

	};
}

#endif // LYXCOOKIE_H
