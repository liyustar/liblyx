#include "lyxThreadTarget.h"

namespace lyx {

ThreadTarget::ThreadTarget(Callback method): _method(method) {
}

ThreadTarget::ThreadTarget(const ThreadTarget& te): _method(te._method) {
}

ThreadTarget& ThreadTarget::operator = (const ThreadTarget& te) {
    _method = te._method;
    return *this;
}

ThreadTarget::~ThreadTarget() {
}

} // namespace lyx
