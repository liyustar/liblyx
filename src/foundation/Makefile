include ../projdef.mk

MODULE := foundation

# 创建cpp的文件依赖
CPPFILES := $(wildcard *.cpp)
DEPINC := $(patsubst %.cpp, %.d, $(CPPFILES))
-include $(DEPINC)

# must define - OBJ
OBJ := $(patsubst %.cpp, %.o, $(CPPFILES))

# PRODUCT 为 liblyx.a
all: $(PRODUCT)

$(PRODUCT): $(OBJ)

clean:
	-rm -f $(OBJ) $(DEPINC)
