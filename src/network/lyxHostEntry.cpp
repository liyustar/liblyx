#include "lyxHostEntry.h"

namespace lyx {

HostEntry::HostEntry() {
}

HostEntry::HostEntry(struct hostent* entry) {
}

HostEntry::HostEntry(struct addrinfo* info) {
}

HostEntry::HostEntry(const HostEntry& entry) {
}

HostEntry& HostEntry::operator = (const HostEntry& entry) {
    return *this;
}

HostEntry::~HostEntry() {
}

} // namespace lyx
