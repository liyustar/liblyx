#include "lyxMessageHeader.h"
#include "lyxAscii.h"
#include "lyxString.h"
#include "lyxNetException.h"
#include "lyxBugcheck.h"

namespace lyx {

MessageHeader::MessageHeader():
    _fieldLimit(DFL_FIELD_LIMIT)
{
}

MessageHeader::MessageHeader(const MessageHeader& messageHeader):
    NameValueCollection(messageHeader),
    _fieldLimit(DFL_FIELD_LIMIT)
{
}

MessageHeader::~MessageHeader() {
}

MessageHeader& MessageHeader::operator = (const MessageHeader& messageHeader) {
    NameValueCollection::operator = (messageHeader);
    return *this;
}

void MessageHeader::write(std::ostream& ostr) const {
    NameValueCollection::ConstIterator it = begin();
    while (it != end()) {
        ostr << it->first << ": " << it->second << "\r\n";
        ++it;
    }
}

void MessageHeader::read(std::istream& istr) {
    static const int eof = std::char_traits<char>::eof();
    std::streambuf& buf = *istr.rdbuf();

    std::string name;
    std::string value;
    name.reserve(32);
    value.reserve(64);
    int ch = buf.sbumpc();
    int fields = 0;

    // 单条消息开头，不为结束符号
    while (ch != eof && ch != '\r' && ch != '\n') {
        if (_fieldLimit > 0 && fields == _fieldLimit)
            throw MessageException("Too many header fields");

        name.clear();
        value.clear();

        // 得到name值
        while (ch != eof && ch != ':' && ch != '\n' && name.length() < MAX_NAME_LENGTH) {
            name += ch;
            ch = buf.sbumpc();
        }
        if (ch == '\n') {
            ch = buf.sbumpc();  // 吃掉'\n'
            continue;           // ignore invalid header lines
        }
        if (ch != ':') throw MessageException("Field name too long/no colon found");
        if (ch != eof) ch = buf.sbumpc();

        // 吃掉空格
        while (ch != eof && Ascii::isSpace(ch) && ch != '\r' && ch != '\n') ch = buf.sbumpc();
        // 得到value
        while (ch != eof && ch != '\r' && ch != '\n' && value.length() < MAX_VALUE_LENGTH) {
            value += ch;
            ch = buf.sbumpc();
        }
        if (ch == '\r') ch = buf.sbumpc();
        if (ch == '\n')
            ch = buf.sbumpc();
        else if (ch != eof)
            throw MessageException("Field value too long/no CRLF found");

        // 处理value换行
        while (ch == ' ' || ch == '\t') {
            while (ch != eof && ch != '\r' && ch != '\n' && value.length() < MAX_VALUE_LENGTH) {
                value += ch;
                ch = buf.sbumpc();
            }
            if (ch == '\r') ch = buf.sbumpc();
            if (ch == '\n')
                ch = buf.sbumpc();
            else if (ch != eof)
                throw MessageException("Folded field value too long/no CRLF found");
        }
        trimInPlace(value);
        add(name, value);
        ++fields;
    }
    istr.putback(ch);
}

int MessageHeader::getFieldLimit() const {
    return _fieldLimit;
}

void MessageHeader::setFieldLimit(int limit) {
    lyx_assert (limit >= 0);

    _fieldLimit = limit;
}

bool MessageHeader::hasToken(const std::string& fieldName, const std::string& token) const {
    std::string field = get(fieldName, "");
    std::vector<std::string> tokens;
    splitElements(field, tokens, true);
    for (std::vector<std::string>::const_iterator it = tokens.begin(); it != tokens.end(); ++it) {
        if (*it == token)
            return true;
    }
    return false;
}

void MessageHeader::splitElements(const std::string& s, std::vector<std::string>& elements, bool ignoreEmpty) {
    elements.clear();
    std::string::const_iterator it  = s.begin();
    std::string::const_iterator end = s.end();
    std::string elem;
    elem.reserve(64);

    throw NotImplementedException("MessageHeader::splitElements() not implement");
}

void MessageHeader::splitParameters(const std::string::const_iterator& begin, const std::string::const_iterator& end, NameValueCollection& parameters) {
    throw NotImplementedException("MessageHeader::splitElements() not implement");
}

void MessageHeader::quote(const std::string& value, std::string& result, bool allowSpace) {
    bool mustQuote = false;
    for (std::string::const_iterator it = value.begin(); !mustQuote && it != value.end(); ++it) {
        if (!lyx::Ascii::isAlphaNumeric(*it) && *it != '.' && *it != '_'
                && *it != '-' && !(lyx::Ascii::isSpace(*it) && allowSpace))
            mustQuote = true;
    }
    if (mustQuote) result += '"';
    result.append(value);
    if (mustQuote) result += '"';
}

} // nemaspace lyx
