#include "lyxLogger.h"

namespace lyx {

Logger::LoggerMap* Logger::_pLoggerMap = 0;
Mutex Logger::_mapMtx;
const std::string Logger::ROOT;

Logger::Logger(const std::string& name, ChannelPtr pChannel, int level):
    _name(name),
    _pChannel(pChannel),
    _level(level)
{
}

Logger::~Logger() {
}

void Logger::setChannel(ChannelPtr pChannel) {
    _pChannel = pChannel;
}

ChannelPtr Logger::getChannel() const {
    return _pChannel;
}

void Logger::setLevel(int level) {
    _level = level;
}

void Logger::setProperty(const std::string& name, const std::string& value) {
    if (name == "channel")
        ;
        // Not Implement;
    else if (name == "level")
        ;
        // setLevel(value);
    else
        Channel::setProperty(name, value);
}

void Logger::log(const Message& msg) {
    if (_level >= msg.getPriority() && _pChannel.get()) {
        _pChannel->log(msg);
    }
}

void Logger::log(const Exception& exc) {
    error(exc.displayText());
}

void Logger::log(const Exception& exc, const char* file, int line) {
    error(exc.displayText(), file, line);
}

} // namespace lyx
