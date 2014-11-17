#include "lyxTimestamp.h"
#include <sys/time.h>
#include <utility>  // for swap
#include "lyxException.h"

namespace lyx {

Timestamp::Timestamp() {
    update();
}

Timestamp::Timestamp(TimeVal tv) {
    _ts = tv;
}

Timestamp::Timestamp(const Timestamp& other) {
    _ts = other._ts;
}

Timestamp::~Timestamp() {
}

Timestamp& Timestamp::operator = (const Timestamp& other) {
    _ts = other._ts;
    return *this;
}

void Timestamp::swap(Timestamp& timestamp) {
    std::swap(_ts, timestamp._ts);
}

Timestamp Timestamp::fromEpochTime(std::time_t t) {
    return Timestamp(TimeVal(t) * resolution());
}

Timestamp Timestamp::fromUtcTime(UtcTimeVal val) {
    val -= (TimeDiff(0x01b21dd2) << 32) + 0x13814000;
    val /= 10;
    return Timestamp(val);
}

void Timestamp::update() {
    struct timeval tv;
    if (gettimeofday(&tv, NULL))
        throw SystemException("cannot get time of day");
    _ts = TimeVal(tv.tv_sec) * resolution() + tv.tv_usec;
}

} // namespace lyx
