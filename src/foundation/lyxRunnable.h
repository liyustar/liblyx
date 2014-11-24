#ifndef LIBLYX_FOUNDATION_LYXRUNNABLE_H_
#define LIBLYX_FOUNDATION_LYXRUNNABLE_H_

namespace lyx {

class Runnable {
    public:
        Runnable() {}
        virtual ~Runnable() {}

        virtual void run() = 0;
};

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXRUNNABLE_H_
