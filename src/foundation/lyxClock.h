#ifndef LIBLYX_FOUNDATION_LYXCLOCK_H_
#define LIBLYX_FOUNDATION_LYXCLOCK_H_

namespace lyx {

class Clock {
    public:
        typedef long long ClockVal;
        typedef long long ClockDiff;

        Clock();
        Clock(ClockVal tv);
        Clock(const Clock& other);
        ~Clock();

        Clock& operator = (const Clock& other);
        Clock& operator = (ClockVal tv);

        void swap(Clock& clock);
        void update();

        bool operator == (const Clock& ts) const;
        bool operator != (const Clock& ts) const;
        bool operator >  (const Clock& ts) const;
        bool operator >= (const Clock& ts) const;
        bool operator <  (const Clock& ts) const;
        bool operator <= (const Clock& ts) const;

        Clock operator + (ClockDiff d) const;
        Clock operator - (ClockDiff d) const;
        Clock& operator += (ClockDiff d);
        Clock& operator -= (ClockDiff d);
        ClockDiff operator - (const Clock& ts) const;

        ClockVal microseconds() const;
        ClockVal raw() const;
        ClockDiff elapsed() const;
        bool isElapsed(ClockDiff interval) const;

        // return the resolution in units per second.
        static ClockVal resolution();
        static ClockVal accuracy();
        static bool monotonic();

    private:
        ClockVal _clock;
};

inline bool Clock::operator == (const Clock& ts) const {
    return _clock == ts._clock;
}

inline bool Clock::operator != (const Clock& ts) const {
    return _clock != ts._clock;
}

inline bool Clock::operator > (const Clock& ts) const {
    return _clock > ts._clock;
}

inline bool Clock::operator >= (const Clock& ts) const {
    return _clock >= ts._clock;
}

inline bool Clock::operator < (const Clock& ts) const {
    return _clock < ts._clock;
}

inline bool Clock::operator <= (const Clock& ts) const {
    return _clock <= ts._clock;
}

inline Clock Clock::operator + (Clock::ClockDiff d) const {
    return Clock(_clock + d);
}

inline Clock Clock::operator - (Clock::ClockDiff d) const {
    return Clock(_clock - d);
}

inline Clock& Clock::operator += (Clock::ClockDiff d) {
    _clock += d;
    return *this;
}

inline Clock& Clock::operator -= (Clock::ClockDiff d) {
    _clock -= d;
    return *this;
}

inline Clock::ClockDiff Clock::operator - (const Clock& ts) const {
    return _clock - ts._clock;
}

inline Clock::ClockVal Clock::microseconds() const {
    return _clock;
}

inline Clock::ClockDiff Clock::elapsed() const {
    Clock now;
    return now - *this;
}

inline bool Clock::isElapsed(Clock::ClockDiff interval) const {
    Clock now;
    Clock::ClockDiff diff = now - *this;
    return diff >= interval;
}

inline Clock::ClockVal Clock::resolution() {
    return 1000000;
}

inline void swap(Clock& s1, Clock& s2) {
    s1.swap(s2);
}

inline Clock::ClockVal Clock::raw() const {
    return _clock;
}

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXCLOCK_H_
