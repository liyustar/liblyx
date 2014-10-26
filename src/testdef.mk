# 给test工程调用的，所以目录是上上上级

INCDIR := ../../../include
LIBDIR := ../../../lib

GTEST_DIR = /home/lyx/Tool/gtest-1.7.0

PRODUCT := $(LIBDIR)/liblyx.a
PRODUCT += $(GTEST_DIR)/make/gtest_main.a

# add modules
MODULES :=	util \
			network \
			http \
			common \
			foundation \
			gtest_example

CPPFLAGS += $(addprefix -I../../,$(MODULES))

# 其他依赖库头文件
CPPFLAGS += -I/usr/local/ssl/include
CPPFLAGS += -isystem $(GTEST_DIR)/include

CXXFLAGS += -std=c++0x -g -Wall -Wextra -pthread

LDFLAGS := -L$(LIBDIR)
LDLIBS := -llyx

# 其他依赖库
LDFLAGS += -L/usr/local/ssl/lib
LDLIBS += -lssl -lcrypto -ldl -lprotobuf
