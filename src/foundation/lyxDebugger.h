#ifndef LIBLYX_FOUNDATION_LYXDEBUGGER_H_
#define LIBLYX_FOUNDATION_LYXDEBUGGER_H_

#include <string>

namespace lyx {

class Debugger {
    public:
        static bool isAvailable();
        static void message(const std::string& msg);
        static void message(const std::string& msg, const char* file, int line);
        static void enter();
        static void enter(const std::string& msg);
        static void enter(const std::string& msg, const char* file, int line);
        static void enter(const char* file, int line);
};

} // namespace lyx

#endif //  LIBLYX_FOUNDATION_LYXDEBUGGER_H_
