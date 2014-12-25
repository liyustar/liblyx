#include "lyxEnvironment.h"

#include "lyxEnvironmentImpl.h"

namespace lyx {

std::string Environment::get(const std::string& name) {
    return EnvironmentImpl::getImpl(name);
}

std::string Environment::get(const std::string& name, const std::string& defaultValue) {
    if (has(name))
        return get(name);
    else
        return defaultValue;
}

bool Environment::has(const std::string& name) {
    EnvironmentImpl::hasImpl(name);
}


} // namespace lyx
