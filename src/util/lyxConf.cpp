#include "lyxConf.h"
#include <string>
#include <iostream>
#include <sstream>
#include "lyxDataStream.h"
#include "lyxConfMsg.pb.h"

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

bool Conf::deleteSection(const string& section) {
    SectionMap::iterator iter = m_secMap.find(section);
    if (iter != m_secMap.end()) {
        m_secMap.erase(iter);
        return true;
    }
    return false;
}

bool Conf::deleteKey(const string& section, const string& key) {
    SectionMap::iterator secIter = m_secMap.find(section);
    if (secIter != m_secMap.end()) {
        KeyValueMap::iterator keyIter = secIter->second.find(key);
        if (keyIter != secIter->second.end()) {
            secIter->second.erase(keyIter);
            return true;
        }
    }
    return false;
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

bool Conf::ParseFromIstream(std::istream& istrm) {
    clear();
    int secSize = DataStream::readInt(istrm);
    std::string section;
    std::string key;
    std::string value;
    for (int i = 0; i < secSize; i++) {
        section = DataStream::readSizeAndBytes(istrm);
        int keySize = DataStream::readInt(istrm);
        for (int j = 0; j < keySize; j++) {
            key = DataStream::readSizeAndBytes(istrm);
            value = DataStream::readSizeAndBytes(istrm);
            setValue(section, key, value);
        }
    }
    return true;
}

bool Conf::SerializeToOstream(std::ostream& ostrm) {
    const Conf::SectionSet sections = getSections();
    Conf::SectionSet::iterator secIter;

    DataStream::writeInt(ostrm, sections.size());   // write section's size
    for (secIter = sections.begin(); secIter != sections.end(); secIter++) {
        const Conf::KeySet keys = getKeys(*secIter);
        Conf::KeySet::iterator keyIter;

        DataStream::writeSizeAndBytes(ostrm, *secIter); // write section
        DataStream::writeInt(ostrm, keys.size());       // write key's size
        for (keyIter = keys.begin(); keyIter != keys.end(); keyIter++) {
            DataStream::writeSizeAndBytes(ostrm, *keyIter); // write key
            DataStream::writeSizeAndBytes(ostrm, getValue(*secIter, *keyIter)); // write value
        }
    }
    return true;
}

bool Conf::PBParseFromIstream(std::istream& istrm) {
    ConfMsg confMsg;
    if (!confMsg.ParseFromIstream(&istrm)) {
        return false;
    }

    for (int i = 0; i < confMsg.section_size(); i++) {
        const ConfMsg::Section& section = confMsg.section(i);
        for (int j = 0; j < section.keyvalue_size(); j++) {
            const ConfMsg::Section::KeyValue& kv = section.keyvalue(j);
            setValue(section.name(), kv.key(), kv.value());
        }
    }
    return true;
}

bool Conf::PBSerializeToOstream(std::ostream& ostrm) {
    ConfMsg confMsg;

    const Conf::SectionSet sections = getSections();
    Conf::SectionSet::iterator secIter;
    for (secIter = sections.begin(); secIter != sections.end(); secIter++) {
        const Conf::KeySet keys = getKeys(*secIter);
        Conf::KeySet::iterator keyIter;

        ConfMsg::Section* sectionMsg = confMsg.add_section();
        sectionMsg->set_name(*secIter);
        for (keyIter = keys.begin(); keyIter != keys.end(); keyIter++) {
            ConfMsg::Section::KeyValue* kvMsg = sectionMsg->add_keyvalue();
            kvMsg->set_key(*keyIter);
            kvMsg->set_value(getValue(*secIter, *keyIter));
        }
    }

    return confMsg.SerializeToOstream(&ostrm);
}

std::string Conf::DumpStr() const {
    std::ostringstream ostrm;
    ostrm << "<Conf>{";
    const Conf::SectionSet sections = getSections();
    Conf::SectionSet::iterator secIter;
    for (secIter = sections.begin(); secIter != sections.end(); secIter++) {
        const Conf::KeySet keys = getKeys(*secIter);
        Conf::KeySet::iterator keyIter;
        ostrm << "[=" << (*secIter) << "=";
        for (keyIter = keys.begin(); keyIter != keys.end(); keyIter++) {
            ostrm << "[" << (*keyIter) << "," << getValue(*secIter, *keyIter) << "],";
        }
        ostrm << "=],";
    }
    ostrm << "}";
    return ostrm.str();
}

std::ostream& operator << (std::ostream& ostrm, const Conf& conf) {
    ostrm << conf.DumpStr();
    return ostrm;
}

}
