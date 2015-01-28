#include "lyxConsoleChannel.h"
#include "lyxMessage.h"
#include "lyxException.h"
#include <iostream>

namespace lyx {

FastMutex ConsoleChannel::_mutex;

ConsoleChannel::ConsoleChannel(): _str(std::clog) {
}

ConsoleChannel::ConsoleChannel(std::ostream& str): _str(str) {
}

ConsoleChannel::~ConsoleChannel() {
}

void ConsoleChannel::log(const Message& msg) {
    FastMutex::ScopedLock lock(_mutex);

    _str << msg.getText() << std::endl;
}

} // namespace lyx
