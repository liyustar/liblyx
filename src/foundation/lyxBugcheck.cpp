#include "lyxBugcheck.h"
#include "lyxException.h"
#include <sstream>

namespace lyx {

void Bugcheck::assertion(const char* cond, const char* file, int line) {
    throw AssertionViolationException(what(cond, file, line));
}

void Bugcheck::nullPointer(const char* ptr, const char* file, int line) {
    throw NullPointerException(what(ptr, file, line));
}

void Bugcheck::bugcheck(const char* file, int line) {
    throw BugcheckException(what(0, file, line));
}

void Bugcheck::bugcheck(const char* msg, const char* file, int line) {
    std::string m("Bugcheck");
    if (msg) {
        m.append(": ");
        m.append(msg);
    }
    throw BugcheckException(what(msg, file, line));
}

void Bugcheck::unexpected(const char* file, int line) {
}

void Bugcheck::debugger(const char* file, int line) {
}

void Bugcheck::debugger(const char* msg, const char* file, int line) {
}

std::string Bugcheck::what(const char* msg, const char* file, int line) {
    std::ostringstream str;
    if (msg) str << msg << " ";
    str << "in file \"" << file << "\", line " << line;
    return str.str();
}

} // namespace lyx
