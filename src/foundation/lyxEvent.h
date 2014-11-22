#ifndef LIBLYX_FOUNDATION_LYXEVENT_H_
#define LIBLYX_FOUNDATION_LYXEVENT_H_

#include "lyxEventImpl.h"
#include "lyxException.h"

namespace lyx {

class Event: private EventImpl {
    public:
        Event(bool autoReset = true);
        ~Event();

        void set();
        void wait();
        void wait(long milliseconds);
        bool tryWait(long milliseconds);
        void reset();

    private:
        Event(const Event&);
        Event& operator = (const Event&);
};

inline void Event::set() {
    setImpl();
}

inline void Event::wait() {
    waitImpl();
}

inline void Event::wait(long milliseconds) {
    if (!waitImpl(milliseconds))
        throw TimeoutException();
}

inline bool Event::tryWait(long milliseconds) {
    return waitImpl(milliseconds);
}

inline void Event::reset() {
    resetImpl();
}

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXEVENT_H_
