#include "lyxDebugger.h"
#include <sstream>
#include <cstdio>
#include <unistd.h>
#include <signal.h>

namespace lyx {

bool Debugger::isAvailable() {
#if defined(_LYX_DEBUG)
    return true;
#else
    return false;
#endif
}

void Debugger::message(const std::string& msg) {
#if defined(_LYX_DEBUG)
    std::fputs("\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n", stderr);
    std::fputs(msg.c_str(), stderr);
    std::fputs("\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n", stderr);
#endif
}

void Debugger::message(const std::string& msg, const char* file, int line) {
#if defined(_LYX_DEBUG)
    std::ostringstream str;
    str << msg << " [in file \"" << file << "\", line " << line << "]";
    message(str.str());
#endif
}

void Debugger::enter() {
#if defined(_LYX_DEBUG)
    kill(getpid(), SIGINT);
#endif
}

void Debugger::enter(const std::string& msg, const char* file, int line) {
#if defined(_LYX_DEBUG)
    message(msg, file, line);
    enter();
#endif
}

void Debugger::enter(const char* file, int line) {
#if defined(_LYX_DEBUG)
    message("BREAK", file, line);
    enter();
#endif
}

} // namespace lyx
