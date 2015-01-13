#ifndef LIBLYX_NETWORK_LYXHTTPCOOKIE_H_
#define LIBLYX_NETWORK_LYXHTTPCOOKIE_H_

#include <string>

namespace lyx {

class NameValueCollection;

class HTTPCookie {
    public:
        HTTPCookie();
        explicit HTTPCookie(const std::string& name);
        explicit HTTPCookie(const NameValueCollection& nvc);
        HTTPCookie(const std::string& name, const std::string& value);
        HTTPCookie(const HTTPCookie& cookie);
        ~HTTPCookie();

        HTTPCookie& operator = (const HTTPCookie& cookie);

        void setVersion(int version);
        int getVersion() const;

        void setName(const std::string& name);
        const std::string& getName() const;

        void setValue(const std::string& value);
        const std::string& getValue() const;

        void setComment(const std::string& comment);
        const std::string& getComment() const;

        void setDomain(const std::string& domain);
        const std::string& getDomain() const;

        void setPath(const std::string& path);
        const std::string& getPath() const;

        void setPriority(const std::string& priority);
        const std::string& getPriority() const;

        void setSecure(bool secure);
        bool getSecure() const;

        void setMaxAge(int maxAge);
        int getMaxAge() const;

        void setHttpOnly(bool flag = true);
        bool getHttpOnly() const;

        std::string toString() const;

        static std::string escape(const std::string& str);
        static std::string unescape(const std::string& str);

    private:
        int         _version;
        std::string _name;
        std::string _value;
        std::string _comment;
        std::string _domain;
        std::string _path;
        std::string _priority;
        bool        _secure;
        int         _maxAge;
        bool        _httpOnly;
};

inline int HTTPCookie::getVersion() const {
    return _version;
}

inline const std::string& HTTPCookie::getName() const {
    return _name;
}

inline const std::string& HTTPCookie::getValue() const {
    return _value;
}

inline const std::string& HTTPCookie::getComment() const {
    return _comment;
}

inline const std::string& HTTPCookie::getDomain() const {
    return _domain;
}

inline const std::string& HTTPCookie::getPath() const {
    return _path;
}

inline const std::string& HTTPCookie::getPriority() const {
    return _priority;
}

inline bool HTTPCookie::getSecure() const {
    return _secure;
}

inline int HTTPCookie::getMaxAge() const {
    return _maxAge;
}

inline bool HTTPCookie::getHttpOnly() const {
    return _httpOnly;
}

} // namespace lyx

#endif // LIBLYX_NETWORK_LYXHTTPCOOKIE_H_
