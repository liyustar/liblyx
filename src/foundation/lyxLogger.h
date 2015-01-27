#ifndef LIBLYX_FOUNDATION_LYXLOGGER_H_
#define LIBLYX_FOUNDATION_LYXLOGGER_H_

#include "lyxChannel.h"
#include "lyxMessage.h"
#include "lyxMutex.h"
#include <vector>

namespace lyx {

class Exception;

class Logger: public Channel {
    public:
        const std::string& name() const;

        void setChannel(ChannelPtr pChannel);
        ChannelPtr getChannel() const;

        void setLevel(int level); // Message::Priority
        int getLevel() const;

        void setProperty(const std::string& name, const std::string& value);

        void log(const Message& msg);
        void log(const Exception& exc);
        void log(const Exception& exc, const char* file, int line);

        void fatal(const std::string& msg);
        void fatal(const std::string& msg, const char* file, int line);

        void critical(const std::string& msg);
        void critical(const std::string& msg, const char* file, int line);

        void error(const std::string& msg);
        void error(const std::string& msg, const char* file, int line);

        void warning(const std::string& msg);
        void warning(const std::string& msg, const char* file, int line);

        void notice(const std::string& msg);
        void notice(const std::string& msg, const char* file, int line);

        void info(const std::string& msg);
        void info(const std::string& msg, const char* file, int line);

        void debug(const std::string& msg);
        void debug(const std::string& msg, const char* file, int line);

        void trace(const std::string& msg);
        void trace(const std::string& msg, const char* file, int line);

        // void dump(const std::string& msg, const void* buffer, std::size_t length, Message::Priority prio = Message::PRIO_DEBUG);

        bool is(int level) const;
        bool fatal() const;
        bool critical() const;
        bool error() const;
        bool warning() const;
        bool notice() const;
        bool info() const;
        bool debug() const;
        bool trace() const;

        /*
        static void setLevel(const std::string& name, int level);
        static void setChannel(const std::string& name, Channel* pChannel);
        static void setProperty(const std::string& loggerName, const std::string& propertyName, const std::string& value);
        static Logger& get(const std::string& name);
        static Logger& unsafeGet(const std::string& name);
        static Logger& create(const std::string& name, Channel* pChannel, int level = Message::PRIO_INFORMATION);
        static Logger& root();
        static Logger* has(const std::string& name);
        static void destroy(const std::string& name);
        static void shutdown();
        static void names(std::vector<std::string>& name);
        */
        static const std::string ROOT;

    protected:
        typedef std::map<std::string, Logger*> LoggerMap;

        Logger(const std::string& name, ChannelPtr pChannel, int level);
        ~Logger();

        void log(const std::string& text, Message::Priority prio);
        void log(const std::string& text, Message::Priority prio, const char* file, int line);

        /*
        static Logger& parent(const std::string& name);
        static void add(Logger* pLogger);
        static Logger* find(const std::string& name);
        */

    private:
        Logger();
        Logger(const Logger&);
        Logger& operator = (const Logger&);

        std::string _name;
        ChannelPtr  _pChannel;
        int         _level;

        static LoggerMap* _pLoggerMap;
        static Mutex      _mapMtx;
};

inline const std::string& Logger::name() const {
    return _name;
}

inline int Logger::getLevel() const {
    return _level;
}

inline void Logger::log(const std::string& text, Message::Priority prio) {
    if (_level >= prio && _pChannel.get()) {
        _pChannel->log(Message(_name, text, prio));
    }
}

inline void Logger::log(const std::string& text, Message::Priority prio, const char* file, int line) {
    if (_level >= prio && _pChannel) {
        _pChannel->log(Message(_name, text, prio, file, line));
    }
}

inline void Logger::fatal(const std::string& msg) {
    log(msg, Message::PRIO_FATAL);
}

inline void Logger::fatal(const std::string& msg, const char* file, int line) {
    log(msg, Message::PRIO_FATAL, file, line);
}

inline void Logger::critical(const std::string& msg) {
    log(msg, Message::PRIO_CRITICAL);
}

inline void Logger::critical(const std::string& msg, const char* file, int line) {
    log(msg, Message::PRIO_CRITICAL, file, line);
}

inline void Logger::error(const std::string& msg) {
    log(msg, Message::PRIO_ERROR);
}

inline void Logger::error(const std::string& msg, const char* file, int line) {
    log(msg, Message::PRIO_ERROR, file, line);
}

inline void Logger::warning(const std::string& msg) {
    log(msg, Message::PRIO_WARNING);
}

inline void Logger::warning(const std::string& msg, const char* file, int line) {
    log(msg, Message::PRIO_WARNING, file, line);
}

inline void Logger::notice(const std::string& msg) {
    log(msg, Message::PRIO_NOTICE);
}

inline void Logger::notice(const std::string& msg, const char* file, int line) {
    log(msg, Message::PRIO_NOTICE, file, line);
}

inline void Logger::info(const std::string& msg) {
    log(msg, Message::PRIO_INFORMATION);
}

inline void Logger::info(const std::string& msg, const char* file, int line) {
    log(msg, Message::PRIO_INFORMATION, file, line);
}

inline void Logger::debug(const std::string& msg) {
    log(msg, Message::PRIO_DEBUG);
}

inline void Logger::debug(const std::string& msg, const char* file, int line) {
    log(msg, Message::PRIO_DEBUG, file, line);
}

inline void Logger::trace(const std::string& msg) {
    log(msg, Message::PRIO_TRACE);
}

inline void Logger::trace(const std::string& msg, const char* file, int line) {
    log(msg, Message::PRIO_TRACE, file, line);
}

inline bool Logger::is(int level) const {
    return _level >= level;
}

inline bool Logger::fatal() const {
    return _level >= Message::PRIO_FATAL;
}

inline bool Logger::critical() const {
    return _level >= Message::PRIO_CRITICAL;
}

inline bool Logger::error() const {
    return _level >= Message::PRIO_ERROR;
}

inline bool Logger::warning() const {
    return _level >= Message::PRIO_WARNING;
}

inline bool Logger::notice() const {
    return _level >= Message::PRIO_NOTICE;
}

inline bool Logger::info() const {
    return _level >= Message::PRIO_INFORMATION;
}

inline bool Logger::debug() const {
    return _level >= Message::PRIO_DEBUG;
}

inline bool Logger::trace() const {
    return _level >= Message::PRIO_TRACE;
}

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXLOGGER_H_
