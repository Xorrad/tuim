CXX 	  := g++
CXX_FLAGS := -std=c++17 -pedantic-errors -Wall
LDFLAGS   := -L/usr/lib -lstdc++ -lncurses
SRC 	  := $(wildcard examples/*.cpp)
TARGETS   := $(SRC:examples/%.cpp=%)

all: $(TARGETS) clean

%: examples/%.cpp
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o bin/$@ $<
	./bin/$@

clean:
	@rm -f bin/*
