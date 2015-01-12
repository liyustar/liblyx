#ifndef LIBLYX_NETWORK_LYXMEDIATYPE_H_
#define LIBLYX_NETWORK_LYXMEDIATYPE_H_

#include "lyxNameValueCollection.h"

namespace lyx {

class MediaType {
    public:
        MediaType(const std::string& mediaType);
        MediaType(const std::string& type, const std::string& subType);
        MediaType(const MediaType& mediaType);
        ~MediaType();

        MediaType& operator = (const MediaType& mediaType);
        MediaType& operator = (const std::string& mediaType);

        void swap(MediaType& mediaType);

        void setType(const std::string& type);
        const std::string& getType() const;
        void setSubType(const std::string& subType);
        const std::string& getSubType() const;
        void setParameter(const std::string& name, const std::string& value);
        const std::string& getParameter(const std::string& name) const;
        bool hasParameter(const std::string& name) const;
        void removeParameter(const std::string& name);

        const NameValueCollection& parameters() const;
        std::string toString() const;
        bool matches(const MediaType& mediaType) const;
        bool matches(const std::string& type, const std::string& subType) const;
        bool matches(const std::string& type) const;
        bool matchesRange(const MediaType& mediaType) const;
        bool matchesRange(const std::string& type, const std::string& subType) const;
        bool matchesRange(const std::string& type) const;

    protected:
        void parse(const std::string& mediaType);

    private:
        MediaType();

        std::string         _type;
        std::string         _subType;
        NameValueCollection _parameters;
};

inline const std::string& MediaType::getType() const {
    return _type;
}

inline const std::string& MediaType::getSubType() const {
    return _subType;
}

inline const NameValueCollection& MediaType::parameters() const {
    return _parameters;
}

inline void swap(MediaType& m1, MediaType& m2) {
    m1.swap(m2);
}

} // namespace lyx

#endif // LIBLYX_NETWORK_LYXMEDIATYPE_H_
