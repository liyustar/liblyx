#include "lyxMyChannel.h"
#include "lyxMessage.h"
#include "lyxException.h"
#include <iostream>

namespace lyx {

FastMutex MyChannel::_mutex;

MyChannel::MyChannel(): _str(std::clog) {
}

MyChannel::MyChannel(std::ostream& str): _str(str) {
}

MyChannel::~MyChannel() {
}

void MyChannel::log(const Message& msg) {
    FastMutex::ScopedLock lock(_mutex);

    _str << '<' << msg.getPriority() << '>' <<
        " " << msg.getTime().epochMicroseconds() <<
        " " << msg.getText() << std::endl;
}

} // namespace lyx
