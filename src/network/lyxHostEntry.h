#ifndef LIBLYX_NETWORK_LYXHOSTENTRY_H_
#define LIBLYX_NETWORK_LYXHOSTENTRY_H_

#include <string>
#include <vector>
#include "lyxSocketDefs.h"
#include "lyxIPAddress.h"

namespace lyx {

class HostEntry {
    public:
        typedef std::vector<std::string> AliasList;
        typedef std::vector<IPAddress> AddressList;

        HostEntry();
        HostEntry(struct hostent* entry);
        HostEntry(struct addrinfo* info);
        HostEntry(const HostEntry& entry);
        HostEntry& operator = (const HostEntry& entry);
        ~HostEntry();

        const std::string& name() const;
        const AliasList& aliases() const;
        const AddressList& addresses() const;

    private:
        std::string _name;
        AliasList   _aliases;
        AddressList _addresses;
};

inline const std::string& HostEntry::name() const {
    return _name;
}

inline const HostEntry::AliasList& HostEntry::aliases() const {
    return _aliases;
}

inline const HostEntry::AddressList& HostEntry::addresses() const {
    return _addresses;
}

} // namespace lyx

#endif // LIBLYX_NETWORK_LYXHOSTENTRY_H_
