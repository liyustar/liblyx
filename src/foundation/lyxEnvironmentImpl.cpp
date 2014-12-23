#include "lyxEnvironmentImpl.h"

#include <cstdlib>
#include <sys/utsname.h>

namespace lyx {

EnvironmentImpl::StringMap EnvironmentImpl::_map;
FastMutex EnvironmentImpl::_mutex;

std::string EnvironmentImpl::getImpl(const std::string& name) {
    FastMutex::ScopedLock lock(_mutex);

    const char* val = std::getenv(name.c_str());
    if (val)
        return std::string(val);
    else
        throw NotFoundException(name);
}

bool EnvironmentImpl::hasImpl(const std::string& name) {
    FastMutex::ScopedLock lock(_mutex);

    return std::getenv(name.c_str()) != 0;
}

void EnvironmentImpl::setImpl(const std::string& name, const std::string& value) {
    FastMutex::ScopedLock lock(_mutex);

    std::string var = name;
    var.append("=");
    var.append(value);
    _map[name] = var;
    if (putenv((char*) _map[name].c_str())) {
        std::string msg = "cannot set environment variable: ";
        msg.append(name);
        throw SystemException(msg);
    }
}

std::string EnvironmentImpl::osNameImpl() {
    struct utsname uts;
    uname(&uts);
    return uts.sysname;
}

std::string EnvironmentImpl::osDisplayNameImpl() {
    return osNameImpl();
}

std::string EnvironmentImpl::osVersionImpl() {
    struct utsname uts;
    uname(&uts);
    return uts.machine;
}

std::string EnvironmentImpl::osArchitectureImpl() {
    struct utsname uts;
    uname(&uts);
    return uts.machine;
}

std::string EnvironmentImpl::nodeNameImpl() {
    struct utsname uts;
    uname(&uts);
    return uts.nodename;
}

unsigned EnvironmentImpl::processorCountImpl() {
    throw NotImplementedException();
}

void EnvironmentImpl::nodeIdImpl(NodeId& id) {
    throw NotImplementedException();
}

} // namespace lyx
