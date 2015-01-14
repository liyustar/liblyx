#ifndef LIBLYX_NETWORK_LYXHTTPRESPONSE_H_
#define LIBLYX_NETWORK_LYXHTTPRESPONSE_H_

#include "lyxHTTPMessage.h"
#include "lyxHTTPCookie.h"
#include "lyxTimestamp.h"

namespace lyx {

class HTTPCookie;

class HTTPResponse: public HTTPMessage {
    public:
        enum HTTPStatus {
            HTTP_OK                              = 200,
            HTTP_NO_CONTENT                      = 204,
            HTTP_SEE_OTHER                       = 303,
            HTTP_NOT_MODIFIED                    = 304,
            HTTP_NOT_FOUND                       = 404,
            HTTP_REQUESTED_RANGE_NOT_SATISFIABLE = 416,
        };

        HTTPResponse();
        HTTPResponse(HTTPStatus status, const std::string& reason);
        HTTPResponse(const std::string& version, HTTPStatus status, const std::string& reason);
        HTTPResponse(HTTPStatus status);
        HTTPResponse(const std::string& version, HTTPStatus status);
        virtual ~HTTPResponse();

        void setStatus(HTTPStatus status);
        HTTPStatus getStatus() const;
        void setStatus(const std::string& status);

        void setReason(const std::string& reason);
        const std::string& getReason() const;

        void setStatusAndReason(HTTPStatus status, const std::string& reason);
        void setStatusAndReason(HTTPStatus status);

        void setDate(const Timestamp& dateTime);
        Timestamp getDate() const;

        void addCookie(const HTTPCookie& cookie);
        void getCookies(std::vector<HTTPCookie>& cookies) const;

        void write(std::ostream& ostr) const;
        void read(std::istream& istr);

        static const std::string& getReasonForStatus(HTTPStatus status);

        static const std::string HTTP_REASON_OK;
        static const std::string HTTP_REASON_NO_CONTENT;
        static const std::string HTTP_REASON_SEE_OTHER;
        static const std::string HTTP_REASON_NOT_MODIFIED;
        static const std::string HTTP_REASON_NOT_FOUND;
        static const std::string HTTP_REASON_REQUESTED_RANGE_NOT_SATISFIABLE;
        static const std::string HTTP_REASON_UNKNOWN;

        static const std::string DATE;
        static const std::string SET_COOKIE;

    private:
        enum Limits {
            MAX_VERSION_LENGTH = 8,
            MAX_STATUS_LENGTH  = 3,
            MAX_REASON_LENGTH  = 512,
        };

        HTTPStatus  _status;
        std::string _reason;

        HTTPResponse(const HTTPResponse&);
        HTTPResponse* operator = (const HTTPResponse&);
};

inline HTTPResponse::HTTPStatus HTTPResponse::getStatus() const {
    return _status;
}

inline const std::string& HTTPResponse::getReason() const {
    return _reason;
}

} // namespace lyx

#endif // LIBLYX_NETWORK_LYXHTTPRESPONSE_H_
