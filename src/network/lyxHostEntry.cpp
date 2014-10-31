#include "lyxHostEntry.h"

namespace lyx {

HostEntry::HostEntry() {
}

HostEntry::HostEntry(struct hostent* entry) {
}

HostEntry::HostEntry(struct addrinfo* ainfo) {
    if (ainfo == NULL) return;

    for (struct addrinfo* ai = ainfo; ai; ai = ai->ai_next) {
        if (ai->ai_canonname) {
            _name.assign(ai->ai_canonname);
        }
        if (ai->ai_addrlen && ai->ai_addr) {
            switch (ai->ai_addr->sa_family) {
                case AF_INET:
                    _addresses.push_back(IPAddress(&reinterpret_cast<struct sockaddr_in*>(ai->ai_addr)->sin_addr, sizeof(in_addr)));
                    break;
            }
        }
    }
}

HostEntry::HostEntry(const HostEntry& entry) :
    _name(entry._name),
    _aliases(entry._aliases),
    _addresses(entry._addresses)
{
}

HostEntry& HostEntry::operator = (const HostEntry& entry) {
    if (&entry != this) {
        _name = entry._name;
        _aliases = entry._aliases;
        _addresses = entry._addresses;
    }
    return *this;
}

HostEntry::~HostEntry() {
}

} // namespace lyx
