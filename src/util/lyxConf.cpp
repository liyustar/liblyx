#include "lyxConf.h"
#include <string>
#include <iostream>

namespace lyx {

void Conf::setValue(const string& section, const string& key, const string& value) {
    m_secMap[section][key] = value;
}

std::string Conf::getValue(const string& section, const string& key) {
    return m_secMap[section][key];
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

std::ostream& operator << (std::ostream& ostrm, const Conf& conf) {
    ostrm << "[Not Implement]";
    return ostrm;
}

}
