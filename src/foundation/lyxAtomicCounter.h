#ifndef LIBLYX_FOUNDATION_LYXATOMICCOUNTER_H_
#define LIBLYX_FOUNDATION_LYXATOMICCOUNTER_H_

namespace lyx {

class AtomicCounter {
    public:
        typedef int ValueType;

        AtomicCounter();
        explicit AtomicCounter(ValueType initialValue);
        AtomicCounter(const AtomicCounter& counter);
        ~AtomicCounter();

        AtomicCounter& operator = (const AtomicCounter& counter);
        AtomicCounter& operator = (ValueType value);

        operator ValueType () const;
        ValueType value() const;
        ValueType operator ++ ();
        ValueType operator ++ (int);
        ValueType operator -- ();
        ValueType operator -- (int);

        bool operator ! () const;

    private:
        typedef int ImplType;

        ImplType _counter;
};

inline AtomicCounter::operator AtomicCounter::ValueType () const {
    return _counter;
}

inline AtomicCounter::ValueType AtomicCounter::value() const {
    return _counter;
}

inline AtomicCounter::ValueType AtomicCounter::operator ++ () {
    return __sync_add_and_fetch(&_counter, 1);
}

inline AtomicCounter::ValueType AtomicCounter::operator ++ (int) {
    return __sync_fetch_and_add(&_counter, 1);
}

inline AtomicCounter::ValueType AtomicCounter::operator -- () {
    return __sync_sub_and_fetch(&_counter, 1);
}

inline AtomicCounter::ValueType AtomicCounter::operator -- (int) {
    return __sync_fetch_and_sub(&_counter, 1);
}

inline bool AtomicCounter::operator ! () const {
    return _counter == 0;
}

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXATOMICCOUNTER_H_
