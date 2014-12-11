#include "lyxStopwatch.h"

namespace lyx {

Stopwatch::Stopwatch(): _elapsed(0), _running(false) {
}

Stopwatch::~Stopwatch() {
}

Clock::ClockDiff Stopwatch::elapsed() const {
    if (_running) {
        Clock current;
        return _elapsed + (current - _start);
    }
    else {
        return _elapsed;
    }
}

void Stopwatch::reset() {
    _elapsed = 0;
    _running = false;
}

void Stopwatch::restart() {
    _elapsed = 0;
    _start.update();
    _running = true;
}

} // namespace lyx
