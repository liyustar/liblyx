#include "lyxDNS.h"
#include <cstring>

namespace lyx {

HostEntry DNS::hostByName(const std::string& hostname, unsigned hintFlags) {
    struct addrinfo* pai;
    struct addrinfo hints;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_flags = hintFlags;
    int rc = getaddrinfo(hostname.c_str(), NULL, &hints, &pai);
    if (rc == 0) {
        HostEntry result(pai);
        freeaddrinfo(pai);
        return result;
    }
    else {
        return HostEntry();
    }
}

} // namespace lyx
