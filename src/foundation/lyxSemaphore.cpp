#include "lyxSemaphore.h"

namespace lyx {

Semaphore::Semaphore(int n): SemaphoreImpl(n, n) {
}

Semaphore::Semaphore(int n, int max): SemaphoreImpl(n, max) {
}

Semaphore::~Semaphore() {
}

} // namespace lyx
