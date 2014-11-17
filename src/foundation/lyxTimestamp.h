#ifndef LIBLYX_FOUNDATION_LYXTIMESTAMP_H_
#define LIBLYX_FOUNDATION_LYXTIMESTAMP_H_

#include <ctime>    // for time_t

namespace lyx {

class Timestamp {
    public:
        typedef long long TimeVal;
        typedef long long UtcTimeVal;
        typedef long long TimeDiff;

        Timestamp();
        Timestamp(TimeVal tv);
        Timestamp(const Timestamp& other);
        ~Timestamp();

        Timestamp& operator = (const Timestamp& other);
        Timestamp& operator = (TimeVal tv);

        void swap(Timestamp& timestamp);

        void update();

        Timestamp   operator +  (TimeDiff d) const;
        Timestamp   operator -  (TimeDiff d) const;
        Timestamp   operator += (TimeDiff d);
        Timestamp   operator -= (TimeDiff d);
        TimeDiff    operator -  (const Timestamp& ts) const;

        std::time_t epochTime() const;
        UtcTimeVal utcTime() const;
        TimeVal epochMicroseconds() const;
        TimeDiff elapsed() const;

        static Timestamp fromEpochTime(std::time_t t);
        static Timestamp fromUtcTime(UtcTimeVal val);
        static TimeVal resolution();

    private:
        TimeVal _ts;
};

inline Timestamp Timestamp::operator +  (TimeDiff d) const {
    return Timestamp(_ts + d);
}

inline Timestamp Timestamp::operator -  (TimeDiff d) const {
    return Timestamp(_ts - d);
}

inline Timestamp::TimeDiff Timestamp::operator -  (const Timestamp& ts) const {
    return _ts - ts._ts;
}

inline Timestamp Timestamp::operator += (TimeDiff d) {
    _ts += d;
    return *this;
}

inline Timestamp Timestamp::operator -= (TimeDiff d) {
    _ts -= d;
    return *this;
}

inline std::time_t Timestamp::epochTime() const {
    return std::time_t(_ts/resolution());
}

inline Timestamp::TimeVal Timestamp::elapsed() const {
    Timestamp now;
    return now - *this;
}

inline Timestamp::TimeVal Timestamp::resolution() {
    return 1000000;
}

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXTIMESTAMP_H_
