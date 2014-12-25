#ifndef LIBLYX_FOUNDATION_LYXENVIRONMENT_H_
#define LIBLYX_FOUNDATION_LYXENVIRONMENT_H_

#include <string>

namespace lyx {

class Environment {
    public:
        typedef uint8_t NodeId[6]; // Ethernet address.

        static std::string get(const std::string& name);
        static bool has(const std::string& name);
        static void set(const std::string& name, const std::string& value);
        static std::string osName();
        static std::string osDisplayName();
        static std::string osVersion();
        static std::string osArchitecture();
        static std::string nodeName();
        static void nodeId(NodeId& id);
        static std::string nodeId();
        static unsigned processorCount();
};

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXENVIRONMENT_H_
