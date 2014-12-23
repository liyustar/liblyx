#ifndef LIBLYX_FOUNDATION_LYXENVIRONMENTIMPL_H_
#define LIBLYX_FOUNDATION_LYXENVIRONMENTIMPL_H_

#include "lyxMutex.h"
#include <string>
#include <map>

namespace lyx {

class EnvironmentImpl {
    public:
        typedef uint8_t NodeId[6]; // Ethernet address.

        static std::string getImpl(const std::string& name);
        static bool hasImpl(const std::string& name);
        static void setImpl(const std::string& name, const std::string& value);
        static std::string osNameImpl();
        static std::string osDisplayNameImpl();
        static std::string osVersionImpl();
        static std::string osArchitectureImpl();
        static std::string nodeNameImpl();
        static void nodeIdImpl(NodeId& id);
        static unsigned processorCountImpl();

    private:
        typedef std::map<std::string, std::string> StringMap;

        static StringMap _map;
        static FastMutex _mutex;
};

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXENVIRONMENTIMPL_H_
