#ifndef LIBLYX_FOUNDATION_LYXSIGNALHANDLER_H_
#define LIBLYX_FOUNDATION_LYXSIGNALHANDLER_H_

#include <vector>
#include <setjmp.h>

namespace lyx {

class SignalHandler {
    public:
        SignalHandler();
        ~SignalHandler();

        sigjmp_buf& jumpBuffer();

        static void throwSignalException(int sig);
        static void install();

    protected:
        static void handleSignal(int sig);

        struct JumpBuffer {
            sigjmp_buf buf;
        };
        typedef std::vector<JumpBuffer> JumpBufferVec;
        static JumpBufferVec& jumpBufferVec();

    private:
        static JumpBufferVec _jumpBufferVec;

        friend class ThreadImpl;
};

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXSIGNALHANDLER_H_
