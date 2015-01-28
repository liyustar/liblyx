#ifndef LIBLYX_FOUNDATION_LYXCHANNEL_H_
#define LIBLYX_FOUNDATION_LYXCHANNEL_H_

#include "lyxConfigurable.h"
#include <memory>

namespace lyx {


class Message;
class Channel;

typedef std::shared_ptr<Channel> ChannelPtr;

class Channel: public Configurable {
    public:
        Channel();
        virtual ~Channel();

        virtual void open();
        virtual void close();
        virtual void log(const Message& msg) = 0;
        void setProperty(const std::string& name, const std::string& value);
        std::string getProperty(const std::string& name) const;


    private:
        Channel(const Channel&);
        Channel& operator = (const Channel&);
};

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXCHANNEL_H_
