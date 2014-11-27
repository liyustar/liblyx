#ifndef LIBLYX_FOUNDATION_LYXTIMESPAN_H_
#define LIBLYX_FOUNDATION_LYXTIMESPAN_H_

#include "lyxTimestamp.h"

namespace lyx {

class Timespan {
    public:
        typedef Timestamp::TimeDiff TimeDiff;

        Timespan();
        Timespan(TimeDiff microseconds);
        Timespan(long seconds, long microseconds);
        Timespan(int days, int hours, int minutes, int seconds, int microseconds);
        Timespan(const Timespan& timespan);

        ~Timespan();
        Timespan& operator = (const Timespan& timespan);
        Timespan& operator = (const TimeDiff& microseconds);

        Timespan& assign(long seconds, long microseconds);
        Timespan& assign(int days, int hours, int minutes, int seconds, int microseconds);

        void swap(Timespan& timespan);

        bool operator == (const Timespan& ts) const;
        bool operator != (const Timespan& ts) const;
        bool operator >  (const Timespan& ts) const;
        bool operator >= (const Timespan& ts) const;
        bool operator <  (const Timespan& ts) const;
        bool operator <= (const Timespan& ts) const;

        Timespan operator + (const Timespan& d) const;
        Timespan operator - (const Timespan& d) const;
        Timespan& operator += (const Timespan& d);
        Timespan& operator -= (const Timespan& d);

        int days() const;
        int hours() const;
        int totalHours() const;
        int minutes() const;
        int totalMinutes() const;
        int seconds() const;
        int totalSeconds() const;
        int milliseconds() const;
        TimeDiff totalMilliseconds() const;
        int microseconds() const;
        int useconds() const;
        TimeDiff totalMicroseconds() const;

        static const TimeDiff MILLISECONDS;
        static const TimeDiff SECONDS;
        static const TimeDiff MINUTES;
        static const TimeDiff HOURS;
        static const TimeDiff DAYS;

    private:
        TimeDiff _span;
};

inline int Timespan::days() const {
    return int(_span/DAYS);
}

inline int Timespan::hours() const {
    return int((_span/HOURS) % 24);
}

inline int Timespan::totalHours() const {
    return int(_span/HOURS);
}

inline int Timespan::minutes() const {
    return int((_span/MINUTES) % 60);
}

inline int Timespan::totalMinutes() const {
    return int(_span/MINUTES);
}

inline int Timespan::seconds() const {
    return int((_span/SECONDS) % 60);
}

inline int Timespan::totalSeconds() const {
    return int(_span/SECONDS);
}

inline int Timespan::milliseconds() const {
    return int((_span/MILLISECONDS) % 1000);
}

inline Timespan::TimeDiff Timespan::totalMilliseconds() const {
    return _span/MILLISECONDS;
}

inline int Timespan::microseconds() const {
    return int(_span % 1000);
}

inline int Timespan::useconds() const {
    return int(_span % 1000000);
}

inline Timespan::TimeDiff Timespan::totalMicroseconds() const {
    return _span;
}

inline bool Timespan::operator == (const Timespan& ts) const {
    return _span == ts._span;
}

inline bool Timespan::operator != (const Timespan& ts) const {
    return _span != ts._span;
}

inline bool Timespan::operator > (const Timespan& ts) const {
    return _span > ts._span;
}

inline bool Timespan::operator >= (const Timespan& ts) const {
    return _span >= ts._span;
}

inline bool Timespan::operator < (const Timespan& ts) const {
    return _span < ts._span;
}

inline bool Timespan::operator <= (const Timespan& ts) const {
    return _span <= ts._span;
}

inline void swap(Timespan& s1, Timespan& s2) {
    s1.swap(s2);
}

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXTIMESPAN_H_
