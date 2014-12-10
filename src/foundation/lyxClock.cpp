#include "lyxClock.h"
#include "lyxTimestamp.h"
#include <utility>

namespace lyx {

Clock::Clock() {
    update();
}

Clock::Clock(ClockVal tv) {
    _clock = tv;
}

Clock::Clock(const Clock& other) {
    _clock = other._clock;
}

Clock::~Clock() {
}

Clock& Clock::operator = (const Clock& other) {
    _clock = other._clock;
    return *this;
}

void Clock::swap(Clock& timestamp) {
    std::swap(_clock, timestamp._clock);
}

void Clock::update() {
    lyx::Timestamp now;
    _clock = now.epochMicroseconds();
}

Clock::ClockVal Clock::accuracy() {
    return 1000;
}

bool Clock::monotonic() {
    return false;
}

} // namespace lyx
