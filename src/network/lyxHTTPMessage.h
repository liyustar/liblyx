#ifndef LIBLYX_NETWORK_LYXHTTPMESSAGE_H_
#define LIBLYX_NETWORK_LYXHTTPMESSAGE_H_

#include "lyxMessageHeader.h"

namespace lyx {

class MediaType;

class HTTPMessage: public MessageHeader {
    public:
        void setVersion(const std::string& version);
        const std::string& getVersion() const;

        void setContentLength(std::streamsize length);
        std::streamsize getContentLength() const;
        bool hasContentLength() const;

        void setTransferEncoding(const std::string& transferEncoding);
        const std::string& getTransferEncoding() const;

        void setChunkedTransferEncoding(bool flag);
        bool getChunkedTransferEncoding() const;

        void setContentType(const std::string& mediaType);
        void setContentType(const MediaType& mediaType);
        const std::string& getContentType() const;

        void setKeepAlive(bool keepAlive);
        bool getKeepAlive() const;

        static const std::string HTTP_1_0;
        static const std::string HTTP_1_1;

        static const std::string IDENTITY_TRANSFER_ENCODING;
        static const std::string CHUNKED_TRANSFER_ENCODING;

        static const int         UNKNOWN_CONTENT_LENGTH;
        static const std::string UNKNOWN_CONTENT_TYPE;

        static const std::string CONTENT_LENGTH;
        static const std::string CONTENT_TYPE;
        static const std::string TRANSFER_ENCODING;
        static const std::string CONNECTION;

        static const std::string CONNECT_KEEP_ALIVE;;
        static const std::string CONNECT_CLOSE;

        static const std::string EMPTY;

    protected:
        HTTPMessage();
        HTTPMessage(const std::string& version);
        virtual ~HTTPMessage();

    private:
        HTTPMessage(const HTTPMessage&);
        HTTPMessage& operator = (const HTTPMessage&);

        std::string _version;
};

inline const std::string& HTTPMessage::getVersion() const {
    return _version;
}

inline bool HTTPMessage::hasContentLength() const {
    return has(CONTENT_LENGTH);
}

} // namespace lyx

#endif // LIBLYX_NETWORK_LYXHTTPMESSAGE_H_
