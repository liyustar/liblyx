#ifndef LIBLYX_FOUNDATION_LYXBUGCHECK_H_
#define LIBLYX_FOUNDATION_LYXBUGCHECK_H_

#include <string>

#if defined(_LYX_DEBUG)
#include <iostream>
#endif // if defined(_LYX_DEBUG)

namespace lyx {

class Bugcheck {
    public:
        static void assertion(const char* cond, const char* file, int line);
        static void nullPointer(const char* ptr, const char* file, int line);
        static void bugcheck(const char* file, int line);
        static void bugcheck(const char* msg, const char* file, int line);
        static void unexpected(const char* file, int line);
        static void debugger(const char* file, int line);
        static void debugger(const char* msg, const char* file, int line);

    protected:
        static std::string what(const char* msg, const char* file, int line);
};

#if defined(_LYX_DEBUG)
#define lyx_assert_dbg(cond) \
    if (!(cond)) lyx::Bugcheck::assertion(#cond, __FILE__, __LINE__); else (void) 0
#else   // if defined(_LYX_DEBUG)
#define lyx_assert_dbg(cond)
#endif  // ifndef(_LYX_DEBUG)

#define lyx_assert(cond) \
    if (!(cond)) lyx::Bugcheck::assertion(#cond, __FILE__, __LINE__); else (void) 0

#define lyx_check_ptr(ptr) \
    if (!(ptr)) lyx::Bugcheck::nullPointer(#ptr, __FILE__, __LINE__); else (void) 0

#define lyx_bugcheck() \
    lyx::Bugcheck::bugcheck(__FILE__, __LINE__);

#define lyx_bugcheck_msg(msg) \
    lyx::Bugcheck::bugcheck(msg, __FILE__, __LINE__);

#define lyx_unexpected() \
    lyx::Bugcheck::unexpected(__FILE__, __LINE__);

#if defined(_LYX_DEBUG)
#define lyx_stdout_dbg(outstr) \
    std::cout << __FILE__ << '(' << std::dec << __LINE__ << "):" << outstr << std::endl;
#else   // if defined(_LYX_DEBUG)
#define lyx_stderr_dbg(outstr)
#endif  // ifndef(_LYX_DEBUG)

#if defined(_LYX_DEBUG)
#define lyx_stderr_dbg(outstr) \
    std::cerr << __FILE__ << '(' << std::dec << __LINE__ << "):" << outstr << std::endl;
#else   // if defined(_LYX_DEBUG)
#define lyx_stderr_dbg(outstr)
#endif  // ifndef(_LYX_DEBUG)

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXBUGCHECK_H_
