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
        void setChannel(Channel* pChannel);
        Channel* getChannel() const;

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

        void dump(const std::string& msg, const void* buffer, std::size_t length, Message::Priority prio = Message::PRIO_DEBUG);

        bool is(int level) const;
        bool fatal() const;
        bool critical() const;
        bool error() const;
        bool warning() const;
        bool notice() const;
        bool info() const;
        bool debug() const;
        bool trace() cosnt;

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
        static const std::string ROOT;

    protected:
        typedef std::map<std::string, Logger*> LoggerMap;

        Logger(const std::string& name, Channel* pChannel, int level);
        ~Logger();

        void log(const std::string& text, Message::Priority prio);
        void log(const std::string& text, Message::Priority prio, const char* file, int line);

        static Logger& parent(const std::string& name);
        static void add(Logger* pLogger);
        static Logger* find(const std::string& name);

    private:
        Logger();
        Logger(const Logger&);
        Logger& operator = (const Logger&);

        std::string _name;
        Channel*    _pChannel;
        int         _level;

        static LoggerMap* _pLoggerMap;
        static Mutex      _mapMtx;
};

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXLOGGER_H_
