#ifndef LIBLYX_FOUNDATION_LYXTIMER_H_
#define LIBLYX_FOUNDATION_LYXTIMER_H_

#include "lyxThread.h"
#include "lyxEvent.h"
#include "lyxMutex.h"
#include "lyxRunnable.h"
#include "lyxClock.h"
#include "lyxThreadPool.h"

namespace lyx {

class AbstractTimerCallback;
class ThreadPool;

class Timer: protected Runnable {
    public:
        Timer(long startInterval = 0, long periodicInterval = 0);
        virtual ~Timer();

        void start(const AbstractTimerCallback& method);
        void start(const AbstractTimerCallback& method, Thread::Priority priority);
        void start(const AbstractTimerCallback& method, ThreadPool& threadPool);
        void start(const AbstractTimerCallback& method, Thread::Priority priority, ThreadPool& threadPool);

        void stop();
        void restart();
        void restart(long milliseconds);
        long getStartInterval() const;
        void setStartInterval(long milliseconds);
        long getPeriodicInterval() const;
        void setPeriodicInterval(long milliseconds);
        long skipped() const;

    protected:
        void run();

    private:
        volatile long _startInterval;
        volatile long _periodicInterval;
        Event         _wakeUp;
        Event         _done;
        long          _skipped;
        AbstractTimerCallback* _pCallback;
        Clock                  _nextInvocation;
        mutable FastMutex      _mutex;

        Timer(const Timer&);
        Timer& operator = (const Timer&);
};

class AbstractTimerCallback {
    public:
        AbstractTimerCallback();
        AbstractTimerCallback(const AbstractTimerCallback& callback);
        virtual ~AbstractTimerCallback();

        AbstractTimerCallback& operator = (const AbstractTimerCallback& callback);

        virtual void invoke(Timer& timer) const = 0;
        virtual AbstractTimerCallback* clone() const = 0;
};

template <class C> class TimerCallback: public AbstractTimerCallback {
    public:
        typedef void (C::*Callback)(Timer&);

        TimerCallback(C& object, Callback method): _pObject(&object), _method(method) {
        }

        TimerCallback(const TimerCallback& callback): _pObject(callback._pObject), _method(callback._method) {
        }

        ~TimerCallback() {
        }

        TimerCallback& operator = (const TimerCallback& callback) {
            if (&callback != this) {
                _pObject = callback._pObject;
                _method = callback._method;
            }
            return *this;
        }

        void invoke(Timer& timer) const {
            (_pObject->*_method)(timer);
        }

        AbstractTimerCallback* clone() const {
            return new TimerCallback(*this);
        }

    private:
        TimerCallback();

        C*          _pObject;
        Callback    _method;
};

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXTIMER_H_
