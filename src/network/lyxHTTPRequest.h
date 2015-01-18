#ifndef LIBLYX_NETWORK_LYXHTTPREQUEST_H_
#define LIBLYX_NETWORK_LYXHTTPREQUEST_H_

#include "lyxHTTPMessage.h"

namespace lyx {

class HTTPRequest: public HTTPMessage {
    public:
        HTTPRequest();
        HTTPRequest(const std::string& version);
        HTTPRequest(const std::string& method, const std::string& uri);
        HTTPRequest(const std::string& method, const std::string& uri, const std::string& version);
        virtual ~HTTPRequest();

        void setMethod(const std::string& method);
        const std::string& getMethod() const;

        void setURI(const std::string& uri);
        const std::string& getURI() const;

        void setHost(const std::string& host);
        void setHost(const std::string& host, uint16_t port);
        const std::string& getHost() const;

        void setCookies(const NameValueCollection& cookies);
        void getCookies(NameValueCollection& cookies) const;

        //bool hasCredentials() const;
        //void getCredentials(std::string& scheme, std::string& authInfo) const;
        //void setCredentials(const std::string& scheme, const std::string& authInfo);

        //bool hasProxyCredentials() const;
        //void getProxyCredentials(std::string& scheme, std::string& authInfo) const;
        //void setProxyCredentials(const std::string& scheme, const std::string& authInfo);

        void write(std::ostream& ostr) const;
        void read(std::istream& istr);

        static const std::string HTTP_GET;
        static const std::string HTTP_HEAD;
        static const std::string HTTP_PUT;
        static const std::string HTTP_POST;
        static const std::string HTTP_OPTIONS;
        static const std::string HTTP_DELETE;
        static const std::string HTTP_TRACE;
        static const std::string HTTP_CONNECT;

        static const std::string HOST;
        static const std::string COOKIE;
        static const std::string AUTHORIZATION;
        static const std::string PROXY_AUTHORIZATION;
        static const std::string UPGRADE;

    protected:
        //void getCredentials(const std::string& header, std::string& scheme, std::string& authInfo) const;

        //void setCredentials(const std::string& header, const std::string& scheme, const std::string& authInfo);

    private:
        enum Limits {
            MAX_METHOD_LENGTH  = 32,
            MAX_URI_LENGTH     = 4096,
            MAX_VERSION_LENGTH = 8
        };

        std::string _method;
        std::string _uri;

        HTTPRequest(const HTTPRequest&);
        HTTPRequest& operator = (const HTTPRequest&);
};

inline const std::string& HTTPRequest::getMethod() const {
    return _method;
}

} // namespace lyx

#endif // LIBLYX_NETWORK_LYXHTTPREQUEST_H_
