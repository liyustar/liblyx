# 给module调用的，所以目录是上上级

INCDIR := ../../include
LIBDIR := ../../lib

PRODUCT := $(LIBDIR)/liblyx.a

# add modules
MODULES	:=	util \
			network \
			http

CPPFLAGS := -g -Wall
CPPFLAGS += $(addprefix -I../,$(MODULES))

# 其他依赖库
CPPFLAGS	+=	-I/usr/local/ssl/include

all:

$(PRODUCT): $(OBJ)
	$(AR) $(ARFLAGS) $@ $^

%.d: %.cpp
	@set -e;rm -f $@; \
	$(CXX) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
