CXXFLAGS := -std=c++20 -Iinclude
LDFLAGS := -lc -lm -lstdc++

all: profilepp

profilepp: profilepp.o
	g++ ${LDFLAGS} -o $@ $^

profilepp.o:examples/profilepp.cpp	
	g++ -c ${CXXFLAGS} -o $@ $^

clean:
	rm *.o
	rm profilepp
	rm profile.json
