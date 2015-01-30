#ifndef LIBLYX_FOUNDATION_LYXFILECHANNEL_H_
#define LIBLYX_FOUNDATION_LYXFILECHANNEL_H_

#include "lyxChannel.h"
#include "lyxMutex.h"
#include "lyxLogFile.h"

namespace lyx {

class FileChannel: public Channel {
    public:
        FileChannel();
        FileChannel(const std::string& path);
        ~FileChannel();

        void open();
        void close();
        void log(const Message& msg);

        void setProperty(const std::string& name, const std::string& value);
        std::string getProperty(const std::string& name) const;

    private:
        std::string _path;
        std::string _times;
        std::string _rotation;
        bool        _flush;
        LogFile*    _pFile;
        FastMutex   _mutex;
};

} // namespace lyx

#endif // LIBLYX_FOUNDATION_LYXFILECHANNEL_H_
