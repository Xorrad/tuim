CXX 	  := g++
CXXFLAGS := -std=c++17 -Wall -Wno-format-security -pedantic-errors
LDFLAGS   := -L/usr/lib -lstdc++ -lm -ldl -ltinfo
SRC 	  := $(wildcard examples/*.cpp)
TARGETS   := $(SRC:examples/%.cpp=%)

all: $(TARGETS) test clean

test: tests/test.cpp
	@clear
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) -o bin/$@ $< $(LDFLAGS)
	./bin/$@

%: examples/%.cpp
	@clear
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) -o bin/$@ $< $(LDFLAGS)
	./bin/$@

clean:
	@rm -f bin/*
