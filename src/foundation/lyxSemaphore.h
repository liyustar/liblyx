#ifndef LIBLYX_FOUNDATION_LYXSEMAPHORE_H_
#define LIBLYX_FOUNDATION_LYXSEMAPHORE_H_

#include "lyxSemaphoreImpl.h"
#include "lyxException.h"

namespace lyx {

class Semaphore: private SemaphoreImpl {
    public:
        Semaphore(int n);
        Semaphore(int n, int max);
        ~Semaphore();

        void set();
        void wait();
        void wait(long milliseconds);
        bool tryWait(long milliseconds);

    private:
        Semaphore();
        Semaphore(const Semaphore&);
        Semaphore& operator = (const Semaphore&);
};

inline void Semaphore::set() {
    setImpl();
}

inline void Semaphore::wait() {
    waitImpl();
}

inline void Semaphore::wait(long milliseconds) {
    if (!waitImpl(milliseconds))
        throw TimeoutException();
}

inline bool Semaphore::tryWait(long milliseconds) {
    return waitImpl(milliseconds);
}

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXSEMAPHORE_H_
