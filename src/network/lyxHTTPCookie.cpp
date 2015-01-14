#include "lyxHTTPCookie.h"
#include "lyxNameValueCollection.h"
#include "lyxString.h"
#include "lyxNetException.h"

namespace lyx {

HTTPCookie::HTTPCookie():
    _version(0),
    _secure(false),
    _maxAge(-1),
    _httpOnly(false)
{
}

HTTPCookie::HTTPCookie(const std::string& name):
    _version(0),
    _name(name),
    _secure(false),
    _maxAge(-1),
    _httpOnly(false)
{
}

HTTPCookie::HTTPCookie(const NameValueCollection& nvc):
    _version(0),
    _secure(false),
    _maxAge(-1),
    _httpOnly(false)
{
    for (NameValueCollection::ConstIterator it = nvc.begin(); it != nvc.end(); ++it) {
        const std::string& name  = it->first;
        const std::string& value = it->second;

        if (icompare(name, "comment") == 0) {
            setComment(value);
        }
        else if (icompare(name, "domain") == 0) {
            setDomain(value);
        }
        else if (icompare(name, "path") == 0) {
            setPath(value);
        }
        else if (icompare(name, "priority") == 0) {
            setPriority(value);
        }
        else if (icompare(name, "max-age") == 0) {
            throw NotImplementedException("HTTPCookie::HTTPCookie max-age");
        }
        else if (icompare(name, "secure") == 0) {
            setSecure(true);
        }
        else if (icompare(name, "expires") == 0) {
            throw NotImplementedException("HTTPCookie::HTTPCookie expires");
        }
        else if (icompare(name, "version") == 0) {
            throw NotImplementedException("HTTPCookie::HTTPCookie version");
        }
        else if (icompare(name, "HttpOnly") == 0) {
            setHttpOnly(true);
        }
        else {
            setName(name);
            setValue(value);
        }
    }
}

HTTPCookie::HTTPCookie(const std::string& name, const std::string& value):
    _version(0),
    _name(name),
    _value(value),
    _secure(false),
    _maxAge(-1),
    _httpOnly(false)
{
}

HTTPCookie::HTTPCookie(const HTTPCookie& cookie):
    _version(cookie._version),
    _name(cookie._name),
    _value(cookie._value),
    _comment(cookie._comment),
    _domain(cookie._domain),
    _path(cookie._path),
    _priority(cookie._priority),
    _secure(cookie._secure),
    _maxAge(cookie._maxAge),
    _httpOnly(cookie._httpOnly)
{
}

HTTPCookie::~HTTPCookie() {
}

HTTPCookie& HTTPCookie::operator = (const HTTPCookie& cookie) {
    if (&cookie != this) {
        _version  = cookie._version;
        _name     = cookie._name;
        _value    = cookie._value;
        _comment  = cookie._comment;
        _domain   = cookie._domain;
        _path     = cookie._path;
        _priority = cookie._priority;
        _secure   = cookie._secure;
        _maxAge   = cookie._maxAge;
        _httpOnly = cookie._httpOnly;
    }
    return *this;
}

void HTTPCookie::setVersion(int version) {
    _version = version;
}

void HTTPCookie::setName(const std::string& name) {
    _name = name;
}

void HTTPCookie::setValue(const std::string& value) {
    _value = value;
}

void HTTPCookie::setComment(const std::string& comment) {
    _comment = comment;
}

void HTTPCookie::setDomain(const std::string& domain) {
    _domain = domain;
}

void HTTPCookie::setPath(const std::string& path) {
    _path = path;
}

void HTTPCookie::setPriority(const std::string& priority) {
    _priority = priority;
}

void HTTPCookie::setSecure(bool secure) {
    _secure = secure;
}

void HTTPCookie::setMaxAge(int maxAge) {
    _maxAge = maxAge;
}

void HTTPCookie::setHttpOnly(bool flag) {
    _httpOnly = flag;
}

std::string HTTPCookie::toString() const {
    std::string result;
    result.reserve(256);

    result.append(_name);
    result.append("=");
    if (_version == 0) {
        // Netscape cookie
        result.append(_value);
        if (!_domain.empty()) {
            result.append("; domain=");
            result.append(_domain);
        }
        if (!_path.empty()) {
            result.append("; path=");
            result.append(_path);
        }
        if (!_priority.empty()) {
            result.append("; Priority=");
            result.append(_priority);
        }
        // if (_maxAge != -1) {
            // Timestamp ts;
            // ts += _maxAge * Timestamp::resolution();
            // result.append("; expires=");
            // DateTimeFormatter::append(result, ts, DateTimeFormat::HTTP_FORMAT);
        // }
        if (_secure) {
            result.append("; secure");
        }
        if (_httpOnly) {
            result.append("; HttpOnly");
        }
    }
    else {
        throw NotImplementedException("HTTPCookie::toString version is not 0");
    }

    return result;
}

namespace {
static const std::string ILLEGAL_CHARS("()[]/|\\',;");
}

std::string HTTPCookie::escape(const std::string& str) {
    std::string result;
    throw NotImplementedException("HTTPCookie::escape not implement");
}

std::string HTTPCookie::unescape(const std::string& str) {
    std::string result;
    throw NotImplementedException("HTTPCookie::unescape not implement");
}

} // namespace lyx
