#ifndef LIBLYX_FOUNDATION_LYXTHREADTARGET_H_
#define LIBLYX_FOUNDATION_LYXTHREADTARGET_H_

#include "lyxRunnable.h"

namespace lyx {

/**
 * This adapter simplifies using static member functions as well as-
 * standalone functions as targets for threads.
 * Note that it is possible to pass those entities directly to Thread::start().
 * This adapter is provided as a convenience for higher abstraction level
 * scenarios where Runnable abstract class is used.
 *
 * For using a non-static member function as a thread target, please
 * see the Runnable Adapter class.
 *
 * Usage:
 *  class MyObject {
 *      static void doSomething() {}
 *  };
 *  ThreadTarget ra(&MyObject::doSomething);
 *  Thread thr;
 *  thr.start(ra);
 *
 * or:
 *
 *  void doSomething() {}
 *
 *  ThreadTarget ra(doSomething);
 *  Thread thr;
 *  thr.start(ra);
 */
class ThreadTarget: public Runnable {
    public:
        typedef void (*Callback)();

        ThreadTarget(Callback method);
        ThreadTarget(const ThreadTarget& te);
        ~ThreadTarget();

        ThreadTarget& operator = (const ThreadTarget& te);

        void run();

    private:
        ThreadTarget();

        Callback _method;
};

inline void ThreadTarget::run() {
    _method();
}

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXTHREADTARGET_H_
