#ifndef LIBLYX_UTIL_LYXCONF_H_
#define LIBLYX_UTIL_LYXCONF_H_

#include <string>
#include <utility>
#include <map>
#include <list>

namespace lyx {

class Conf {
    public:
        typedef std::string string;
        typedef string Section;
        typedef string Key;
        typedef string Value;
        typedef std::pair<Key, Value> KeyValue;

        void setValue(const string& section, const string& key, const string& value);
        string getValue(const string& section, const string& key);

        bool modifyValue(const string& section, const string& key, const string& value);

        void clear();
        bool empty() const;

        bool querySection(const string& section);
        bool queryKey(const string& section, const string& key);

    private:
        typedef std::map<string, string> StringMap;
        StringMap m_mapItem;

        typedef std::pair<Section, Key> Index;
        typedef std::pair<Index, Value> Record;
        typedef std::list<Record> RecordList;
        RecordList m_list;

        string makeIndex(const string& section, const string& key) {
            return section + key;
        }
};

}

#endif // LIBLYX_UTIL_LYXCONF_H_
