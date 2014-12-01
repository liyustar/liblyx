#ifndef LIBLYX_FOUNDATION_LYXTHREADLOCAL_H_
#define LIBLYX_FOUNDATION_LYXTHREADLOCAL_H_

#include <map>

namespace lyx {

/**
 * This is the base class for all objects
 * that the ThreadLocalStorage class manages.
 */
class TLSAbstractSlot {
    public:
        TLSAbstractSlot();
        virtual ~TLSAbstractSlot();
};

/**
 * The Slot template wraps another class
 * so that it can be stored in a ThreadLocalStorage
 * objece. This class is used internally, and you
 * must not create instances of it yourself.
 */
template <class C> class TLSSlot: public TLSAbstractSlot {
    public:
        TLSSlot(): _value() {
        }

        ~TLSSlot() {
        }

        C& value() {
            return _value;
        }

    private:
        TLSSlot(const TLSSlot&);
        TLSSlot& operator = (const TLSSlot&);

        C _value;
};

/**
 * This class manages the local storage for each thread.
 * Never use this calss directly, always use the
 * ThreadLocal template for managing thread local storage.
 */
class ThreadLocalStorage {
    public:
        ThreadLocalStorage();

        ~ThreadLocalStorage();

        // Ruturns the slot for the given key.
        TLSAbstractSlot*& get(const void* key);

        // Ruturns the TLS object for the curent thread.
        static ThreadLocalStorage& current();

        // Clears the current thread's TLS object.
        // Does nothing in the main thread.
        static void clear();

    private:
        typedef std::map<const void*, TLSAbstractSlot*> TLSMap;

        TLSMap _map;

        friend class Thread;
};

/**
 * This template is used to declare type safe thread
 * local variables. It can basically be used like
 * a smart pointer class with the special feature
 * that it references a different object
 * in every thread. The underlying object will
 * be created when it ts referenced for the first
 * time.
 * Every thread only has access to its own
 * thread local data. There is no way for a thread
 * to access another thread's local data.
 */
template <class C> class ThreadLocal {
    public:
        ThreadLocal() {
        }

        ~ThreadLocal() {
        }

        C* operator -> () {
            return &get();
        }

        C& operator * () {
            return get();
        }

        C& get() {
            TLSAbstractSlot*& p = ThreadLocalStorage::current().get(this);
            if (!p)
                p = new Slot;
            return static_cast<Slot*>(p)->value();
        }

    private:
        typedef TLSSlot<C> Slot;
        ThreadLocal(const ThreadLocal&);
        ThreadLocal& operator = (const ThreadLocal&);
};


} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXTHREADLOCAL_H_
