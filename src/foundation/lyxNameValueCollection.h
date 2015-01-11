#ifndef LIBLYX_FOUNDATION_LYXNAMEVALUECOLLECTION_H_
#define LIBLYX_FOUNDATION_LYXNAMEVALUECOLLECTION_H_

#include <string>
#include <map>

namespace lyx {

class NameValueCollection {
    public:
        typedef std::map<std::string, std::string> HeaderMap;
        typedef HeaderMap::iterator         Iterator;
        typedef HeaderMap::const_iterator   ConstIterator;

        NameValueCollection();
        NameValueCollection(const NameValueCollection& nvc);
        ~NameValueCollection();

        NameValueCollection& operator = (const NameValueCollection& nvc);
        void swap(NameValueCollection& nvc);
        const std::string& operator [] (const std::string& name) const;

        void set(const std::string& name, const std::string& value);
        void add(const std::string& name, const std::string& value);
        const std::string& get(const std::string& name) const;
        const std::string& get(const std::string& name, const std::string& defaultValue) const;
        bool has(const std::string& name) const;

        ConstIterator find(const std::string& name) const;
        ConstIterator begin() const;
        ConstIterator end() const;

        bool empty() const;
        int size() const;
        void erase(const std::string& name);
        void clear();

    private:
        HeaderMap _map;
};

inline void swap(NameValueCollection& nvc1, NameValueCollection& nvc2) {
    nvc1.swap(nvc2);
}

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXNAMEVALUECOLLECTION_H_
