#include "lyxConf.h"
#include <string>
#include <iterator>

namespace lyx {

void Conf::setValue(const string& section, const string& key, const string& value) {
    // 更新 m_mapItem
    m_mapItem[makeIndex(section, key)] = value;

    // 更新 m_list
    Index index(section, key);
    RecordList::iterator it;
    for (it = m_list.begin(); it != m_list.end(); ++it) {
        if (index == it->first) {
            it->second = value;
            return;
        }
    }
    m_list.push_back(Record(index, value));
}

std::string Conf::getValue(const string& section, const string& key) {
    StringMap::iterator it = m_mapItem.find( makeIndex(section, key) );
    if (it != m_mapItem.end()) {
        return it->second;
    }
    return string();
}

bool Conf::modifyValue(const string& section, const string& key, const string& value) {
    if (value == getValue(section, key)) {
        return false;
    }
    setValue(section, key, value);
    return true;
}

void Conf::clear() {
    m_mapItem.clear();
    m_list.clear();
}

bool Conf::empty() const {
    return m_mapItem.empty();
}

bool Conf::querySection(const string& section) {
    RecordList::iterator it;
    for (it = m_list.begin(); it != m_list.end(); ++it) {
        if (it->first.first == section) {
            return true;
        }
    }
    return false;
}

bool Conf::queryKey(const string& section, const string& key) {
    return m_mapItem.find( makeIndex(section, key) ) != m_mapItem.end();
}

}
