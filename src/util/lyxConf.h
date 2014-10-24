#ifndef LIBLYX_UTIL_LYXCONF_H_
#define LIBLYX_UTIL_LYXCONF_H_

#include <string>
#include <map>
#include <set>

namespace lyx {

class Conf {
    public:
        typedef std::string string;
        typedef string Section;
        typedef string Key;
        typedef string Value;
        typedef std::set<Section> SectionSet;
        typedef std::set<Key> KeySet;

        void setValue(const string& section, const string& key, const string& value);
        string getValue(const string& section, const string& key) const;

        bool modifyValue(const string& section, const string& key, const string& value);
        bool deleteSection(const string& section);
        bool deleteKey(const string& section, const string& key);

        void clear();
        bool empty() const;

        bool querySection(const string& section);
        bool queryKey(const string& section, const string& key);

        SectionSet getSections() const;
        KeySet getKeys(const string& section) const;

        bool ParseFromIstream(std::istream& istrm);
        bool SerializeToOstream(std::ostream& ostrm);

        bool PBParseFromIstream(std::istream& istrm);
        bool PBSerializeToOstream(std::ostream& ostrm);

        string DumpStr() const;

    private:
        typedef std::map<Key, Value> KeyValueMap;
        typedef std::map<Section, KeyValueMap> SectionMap;
        SectionMap m_secMap;
};

std::ostream& operator << (std::ostream& ostrm, const Conf& conf);

}

#endif // LIBLYX_UTIL_LYXCONF_H_
