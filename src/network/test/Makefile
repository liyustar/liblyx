include ../../testdef.mk

# module_1_test.cpp
CPPFILES := $(wildcard *.cpp)

# target
TESTBIN := $(patsubst %.cpp, %, $(CPPFILES))
TESTBIN := $(filter %_test, $(TESTBIN))

# depend object
OBJ := EchoServer.o

all: $(TESTBIN)

$(TESTBIN): $(PRODUCT) $(OBJ)

clean:
	-rm -f $(TESTBIN) $(OBJ)
