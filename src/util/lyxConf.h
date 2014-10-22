#ifndef LIBLYX_UTIL_LYXCONF_H_
#define LIBLYX_UTIL_LYXCONF_H_

#include <string>
#include <map>

namespace lyx {

class Conf {
    public:
        typedef std::string string;
        typedef string Section;
        typedef string Key;
        typedef string Value;

        void setValue(const string& section, const string& key, const string& value);
        string getValue(const string& section, const string& key);

        bool modifyValue(const string& section, const string& key, const string& value);

        void clear();
        bool empty() const;

        bool querySection(const string& section);
        bool queryKey(const string& section, const string& key);

    private:
        typedef std::map<Key, Value> KeyValueMap;
        typedef std::map<Section, KeyValueMap> SectionMap;
        SectionMap m_secMap;
};

std::ostream& operator << (std::ostream& ostrm, const Conf& conf);

}

#endif // LIBLYX_UTIL_LYXCONF_H_
