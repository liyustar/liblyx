include ../../testdef.mk

# module_1_test.cpp
CPPFILES := $(wildcard *.cpp)

# target
TESTBIN := $(patsubst %.cpp, %, $(CPPFILES))
TESTBIN := $(filter %_test, $(TESTBIN))

all: $(TESTBIN)

$(TESTBIN): $(PRODUCT)

clean:
	-rm $(TESTBIN)
