CXXFLAGS := -std=c++20 -Iinclude --save-temps
LDFLAGS := -lc -lm -lstdc++

all: profilepp

profilepp: example 

example: example.o
	g++ ${LDFLAGS} $^ $@

example.o:examples/example.cpp	
	g++ -c ${CXXFLAGS} $^ $@
