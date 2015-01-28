#ifndef LIBLYX_FOUNDATION_LYXMYCHANNEL_H_
#define LIBLYX_FOUNDATION_LYXMYCHANNEL_H_

#include "lyxChannel.h"
#include "lyxMutex.h"

namespace lyx {

class MyChannel: public Channel {
    public:
        MyChannel();
        MyChannel(std::ostream& str);

        ~MyChannel();

        void log(const Message& msg);

    private:
        std::ostream& _str;
        static FastMutex _mutex;
};

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXMYCHANNEL_H_
