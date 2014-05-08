# 给test工程调用的，所以目录是上上上级

INCDIR := ../../../include
LIBDIR := ../../../lib

PRODUCT := $(LIBDIR)/liblyx.a

# add modules
MODULES	:=	util \
			network \
			http

CPPFLAGS := -g -Wall
CPPFLAGS += $(addprefix -I../../,$(MODULES))

# 其他依赖库头文件
CPPFLAGS	+=	-I/usr/local/ssl/include

LDFLAGS := -L$(LIBDIR)
LDLIBS := -llyx

# 其他依赖库
LDFLAGS += -L/usr/local/ssl/lib
LDLIBS += -lssl -lcrypto -ldl
