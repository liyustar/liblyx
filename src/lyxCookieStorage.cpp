/*************************************************************************
    > File Name: lyxCookieStorage.cpp
    > Author: liyuxing
    > Mail: liyustar@gmail.com 
    > Created Time: 2013年10月11日 星期五 20时49分01秒
 ************************************************************************/

#include "lyxCookieStorage.h"
#include "lyxCookie.h"
#include "lyxUrl.h"
#include <iostream>
#include <map>
#include <set>
using namespace std;

namespace lyx {

	CookieStorageInstence CookieStorage::m_cookiestorage;

	CookieStorageInstence CookieStorage::getCookieStorageInstence() {
		if (m_cookiestorage == NULL) {
			m_cookiestorage = new CookieStorage();
		}
		return m_cookiestorage;
	}

	int CookieStorage::addCookie(Cookie cookie) {
		// TODO: find url in map, add cookie in set;
		CookieSetIter iter = m_cookieSet.begin();
		for ( ; iter != m_cookieSet.end(); iter++) {
			if (cookie == *iter) {
				m_cookieSet.erase(iter);
				break;
			}
		}
		m_cookieSet.insert(cookie);
		return 0;
	}

	CookieSet CookieStorage::getCookiesByUrl(Url url) const {
		CookieSet cookies;
		CookieSet::iterator iter = m_cookieSet.begin();
		for ( ; iter != m_cookieSet.end(); iter++) {
			if (iter->getDomain().compare(url.getHostname()) == 0) {
				cookies.insert(*iter);
			}
		}
		return cookies;
	}

}
