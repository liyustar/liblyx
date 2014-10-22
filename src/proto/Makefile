include ../projdef.mk

MODULE := proto

# 创建proto的文件依赖
PROTOFILES := $(wildcard *.proto)
PBCCFILES := $(patsubst %.proto, %.pb.cc, $(PROTOFILES))
PBHFILES := $(patsubst %.proto, %.pb.h, $(PROTOFILES))

# must define - OBJ
OBJ := $(patsubst %.proto, %.pb.o, $(PROTOFILES))

# PRODUCT 为 liblyx.a
all: $(PRODUCT)

$(PRODUCT): $(OBJ)

clean:
	-rm -f $(OBJ) $(DEPINC) $(PBCCFILES) $(PBHFILES)
