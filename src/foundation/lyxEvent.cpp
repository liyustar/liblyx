#include "lyxEvent.h"

namespace lyx {

Event::Event(bool autoReset):
    EventImpl(autoReset)
{
}

Event::~Event() {
}

} // namespace lyx
