#include <gtest/gtest.h>

#include "lyxThreadPool.h"
#include "lyxRunnableAdapter.h"
#include "lyxException.h"
#include "lyxThread.h"

using namespace lyx;

TEST(ThreadPoolTest, threadpool) {
    ThreadPool pool(2, 3, 3);
    pool.setStackSize(1);

    EXPECT_EQ(2, pool.allocated());
    EXPECT_EQ(0, pool.used());
    EXPECT_EQ(3, pool.capacity());
    EXPECT_EQ(3, pool.available());

    pool.addCapacity(1);
    EXPECT_EQ(2, pool.allocated());
    EXPECT_EQ(0, pool.used());
    EXPECT_EQ(4, pool.capacity());
    EXPECT_EQ(4, pool.available());
}
