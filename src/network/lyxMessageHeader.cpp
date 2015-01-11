#include "lyxMessageHeader.h"
#include "lyxAscii.h"
#include "lyxException.h"
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
        ostr << it->first << ": " << it->second << "\n\r";
        ++it;
    }
}

void MessageHeader::read(std::istream& istr) {
    static const int eof = std::char_traits<char>::eof();
    std::streambuf& buf = *istr.rdbuf();

    std::string name;
    std::string value;

    throw NotImplementedException("MessageHeader::read() not implement");
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
