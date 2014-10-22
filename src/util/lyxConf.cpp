#include "lyxConf.h"
#include <string>
#include <iostream>

namespace lyx {

void Conf::setValue(const string& section, const string& key, const string& value) {
    m_secMap[section][key] = value;
}

std::string Conf::getValue(const string& section, const string& key) const {
    if (m_secMap.find(section) != m_secMap.end()
            && m_secMap.at(section).find(key) != m_secMap.at(section).end()) {
        return m_secMap.at(section).at(key);
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
    m_secMap.clear();
}

bool Conf::empty() const {
    return m_secMap.empty();
}

bool Conf::querySection(const string& section) {
    return ! m_secMap[section].empty();
}

bool Conf::queryKey(const string& section, const string& key) {
    return ! m_secMap[section][key].empty();
}

Conf::SectionSet Conf::getSections() const {
    SectionSet sections;
    SectionMap::const_iterator iter;
    for (iter = m_secMap.begin(); iter != m_secMap.end(); iter++) {
        sections.insert(iter->first); // insert map's key
    }
    return sections;
}

Conf::KeySet Conf::getKeys(const string& section) const {
    KeySet keys;
    KeyValueMap::const_iterator iter;
    const KeyValueMap kvMap = m_secMap.at(section);
    for (iter = kvMap.begin(); iter != kvMap.end(); iter++) {
        keys.insert(iter->first);
    }
    return keys;
}

std::ostream& operator << (std::ostream& ostrm, const Conf& conf) {
    ostrm << "<Conf>{";
    const Conf::SectionSet sections = conf.getSections();
    Conf::SectionSet::iterator secIter;
    for (secIter = sections.begin(); secIter != sections.end(); secIter++) {
        const Conf::KeySet keys = conf.getKeys(*secIter);
        Conf::KeySet::iterator keyIter;
        ostrm << "[=" << (*secIter) << "=";
        for (keyIter = keys.begin(); keyIter != keys.end(); keyIter++) {
            ostrm << "[" << (*keyIter) << "," << conf.getValue(*secIter, *keyIter) << "],";
        }
        ostrm << "=],";
    }
    ostrm << "}";
    return ostrm;
}

}
