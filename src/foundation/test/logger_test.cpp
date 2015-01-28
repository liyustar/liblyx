#include <gtest/gtest.h>

#include "lyxLogger.h"
#include "lyxConsoleChannel.h"
#include "lyxMyChannel.h"

using namespace lyx;

TEST(LoggerTest, testLogger) {
    ChannelPtr pChannel(new ConsoleChannel);
    Logger& root = Logger::root();
    root.setChannel(pChannel);
    EXPECT_EQ(Message::PRIO_INFORMATION, root.getLevel());

    root.info("Informational message");
    root.warning("Warning message");
    root.debug("Debug messgage");

}

TEST(LoggerTest, testMyChannelLogger) {
    ChannelPtr pChannel(new MyChannel);
    Logger& root = Logger::root();
    root.setChannel(pChannel);
    EXPECT_EQ(Message::PRIO_INFORMATION, root.getLevel());

    root.info("Informational message");
    root.warning("Warning message");
    root.debug("Debug messgage");

}
