#ifndef LIBLYX_FOUNDATION_LYXTIMER_H_
#define LIBLYX_FOUNDATION_LYXTIMER_H_

#include "lyxClock.h"

namespace lyx {

class Stopwatch {
    public:
        Stopwatch();
        ~Stopwatch();

        void start();
        void stop();
        void reset();
        void restart();

        Clock::ClockDiff elapsed() const;
        int elapsedSeconds() const;

        static Clock::ClockVal resolution();

    private:
        Stopwatch(const Stopwatch&);
        Stopwatch& operator = (const Stopwatch&);

        Clock               _start;
        Clock::ClockDiff    _elapsed;
        bool                _running;
};

inline void Stopwatch::start() {
    if (!_running) {
        _start.update();
        _running = true;
    }
}

inline void Stopwatch::stop() {
    if (_running) {
        Clock current;
        _elapsed += current - _start;
        _running = false;
    }
}

inline int Stopwatch::elapsedSeconds() const {
    return int(elapsed()/resolution());
}

inline Clock::ClockVal Stopwatch::resolution() {
    return Clock::resolution();
}

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXTIMER_H_
