#ifndef LIBLYX_FOUNDATION_LYXCHANNEL_H_
#define LIBLYX_FOUNDATION_LYXCHANNEL_H_

#include "lyxConfigurable.h"

namespace lyx {

class Message;

class Channel: public Configurable {
    public:
        Channel();
        virtual void open();
        virtual void close();
        virtual void log(const Message& msg) = 0;
        void setProperty(const std::string& name, const std::string& value);
        std::string getProperty(const std::string& name) const;

    protected:
        virtual ~Channel();

    private:
        Channel(const Channel&);
        Channel& operator = (const Channel&);
};

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXCHANNEL_H_
