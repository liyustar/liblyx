#ifndef LIBLYX_FOUNDATION_LYXMESSAGE_H_
#define LIBLYX_FOUNDATION_LYXMESSAGE_H_

#include <string>
#include <map>
#include "lyxTimestamp.h"

namespace lyx {

class Message {
    public:
        enum Priority {
            PRIO_FATAL = 1,
            PRIO_CRITICAL,
            PRIO_ERROR,
            PRIO_WARNING,
            PRIO_NOTICE,
            PRIO_INFORMATION,
            PRIO_DEBUG,
            PRIO_TRACE,
        };

        Message();
        Message(const std::string& source, const std::string& text, Priority prio);
        Message(const std::string& source, const std::string& text, Priority prio, const char* file, int line);
        Message(const Message& msg);
        Message(const Message& msg, const std::string& text);
        ~Message();

        Message& operator = (const Message& msg);
        void swap(Message& msg);

        void setSource(const std::string& src);
        const std::string& getSource() const;

        void setText(const std::string& text);
        const std::string& getText() const;

        void setPriority(Priority prio);
        Priority getPriority() const;

        void setTime(const Timestamp& time);
        const Timestamp& getTime() const;

        void setThread(const std::string& thread);
        const std::string& getThread() const;

        void setTid(long pid);
        long getTid() const;

        void setPid(long pid);
        long getPid() const;

        void setSourceFile(const char* file);
        const char* getSourceFile() const;

        void setSourceLine(int line);
        int getSourceLine() const;

        bool has(const std::string& param) const;
        const std::string& get(const std::string& param) const;
        const std::string& get(const std::string& param, const std::string& defaultValue) const;
        void set(const std::string& param, const std::string& value);
        const std::string& operator [] (const std::string& param) const;
        std::string& operator [] (const std::string& param);

    protected:
        void init();
        typedef std::map<std::string, std::string> StringMap;

    private:
        std::string _source;
        std::string _text;
        Priority    _prio;
        Timestamp   _time;
        int         _tid;
        std::string _thread;
        long        _pid;
        const char* _file;
        int         _line;
        StringMap*  _pMap;
};

inline const std::string& Message::getSource() const {
    return _source;
}

inline const std::string& Message::getText() const {
    return _text;
}

inline Message::Priority Message::getPriority() const {
    return _prio;
}

inline const Timestamp& Message::getTime() const {
    return _time;
}

inline const std::string& Message::getThread() const {
    return _thread;
}

inline long Message::getTid() const {
    return _tid;
}

inline long Message::getPid() const {
    return _pid;
}

inline const char* Message::getSourceFile() const {
    return _file;
}

inline int Message::getSourceLine() const {
    return _line;
}

inline void swap(Message& m1, Message& m2) {
    m1.swap(m2);
}

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXMESSAGE_H_
