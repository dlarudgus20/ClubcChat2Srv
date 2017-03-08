include ../configure.mk

C_SOURCE = $(wildcard *.cpp)
C_OBJECT = $(patsubst %.cpp, build/%.o, $(C_SOURCE))

OUTPUT_FILE = ../build/ClubcChat2Srvd

GCC_OPT += -std=c++0x -fpermissive -DUSE_UNIX_ODBC -DCREATE_EXECUTE_FILE
LINK_OPT += -pthread

all: dirs build/depen.dep $(OUTPUT_FILE)

dirs:
	mkdir -p build

$(OUTPUT_FILE): $(C_OBJECT)
	g++ $(GCC_OPT) $(C_OBJECT) -o $(OUTPUT_FILE) $(LINK_OPT)

build/%.o: %.cpp
	g++ $(GCC_OPT) -c $< -o $@

build/depen.dep: $(C_SOURCE)
	g++ -MM $(GCC_OPT) $(C_SOURCE) > build/depen.dep

clean:
	-rm build/*

ifeq ($(wildcard build/depen.dep), build/depen.dep)
include build/depen.dep
endif

