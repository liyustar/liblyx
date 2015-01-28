#ifndef LIBLYX_FOUNDATION_LYXCONSOLECHANNEL_H_
#define LIBLYX_FOUNDATION_LYXCONSOLECHANNEL_H_

#include "lyxChannel.h"
#include "lyxMutex.h"

namespace lyx {

class ConsoleChannel: public Channel {
    public:
        ConsoleChannel();
        ConsoleChannel(std::ostream& str);

        ~ConsoleChannel();

        void log(const Message& msg);

    private:
        std::ostream& _str;
        static FastMutex _mutex;
};

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXCONSOLECHANNEL_H_
