#ifndef LIBLYX_FOUNDATION_LYXTIMESTAMP_H_
#define LIBLYX_FOUNDATION_LYXTIMESTAMP_H_

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

        void update();

        std::time_t epochTime() const;
        UtcTimeVal utcTime() const;
        TimeVal epochMicroseconds() const;
        TimeDiff elapsed() const;

        static Timestamp fromEpochTime(std::time_t t);
        static Timestamp fromUtcTime(UtTimeVal val);
        static TimeVal resolution();

    private:
        TimeVal _ts;
};

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
