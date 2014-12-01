#include "lyxMutex.h"

namespace lyx {

Mutex::Mutex(bool fast): MutexImpl(fast) {
}

Mutex::~Mutex() {
}

} // namespace lyx
