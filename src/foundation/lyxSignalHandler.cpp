#include "lyxSignalHandler.h"

#include <cstdlib>
#include <signal.h>
#include <lyxException.h>

namespace lyx {

SignalHandler::JumpBufferVec SignalHandler::_jumpBufferVec;

SignalHandler::SignalHandler() {
    JumpBufferVec& jbv = jumpBufferVec();
    JumpBuffer buf;
    jbv.push_back(buf);
}

SignalHandler::~SignalHandler() {
    jumpBufferVec().pop_back();
}

sigjmp_buf& SignalHandler::jumpBuffer() {
    return jumpBufferVec().back().buf;
}

void SignalHandler::throwSignalException(int sig) {
    switch (sig) {
        case SIGILL:
            throw SignalException("Illegal instruction");
        case SIGBUS:
            throw SignalException("Bus error");
        case SIGSEGV:
            throw SignalException("Segmentation violation");
        case SIGSYS:
            throw SignalException("Invalid system call");
        default:
            ;
            // throw SignalException(NumberFormatter::formatHex(sig));
    }
}

void SignalHandler::install() {
    struct sigaction sa;
    sa.sa_handler = handleSignal;
    sa.sa_flags   = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGILL,  &sa, 0);
    sigaction(SIGBUS,  &sa, 0);
    sigaction(SIGSEGV, &sa, 0);
    sigaction(SIGSYS,  &sa, 0);
}

void SignalHandler::handleSignal(int sig) {
    JumpBufferVec& jb = jumpBufferVec();
    if (!jb.empty())
        siglongjmp(jb.back().buf, sig);

    // Abort if no jump buffer registered
    std::abort();
}

SignalHandler::JumpBufferVec& SignalHandler::jumpBufferVec() {
    return _jumpBufferVec;
}

} // namespace lyx
