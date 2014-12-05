#ifndef LIBLYX_FOUNDATION_LYXSINGLETONHOLDER_H_
#define LIBLYX_FOUNDATION_LYXSINGLETONHOLDER_H_

#include "lyxMutex.h"

namespace lyx {

template <class S> class SingletonHolder {
    public:
        SingletonHolder(): _pS(0) {
        }

        ~SingletonHolder() {
            delete _pS;
        }

        S* get() {
            FastMutex::ScopedLock lock(_m);
            if (!_pS) _pS = new S;
            return _pS;
        }

    private:
        S* _pS;
        FastMutex _m;
};

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXSINGLETONHOLDER_H_
