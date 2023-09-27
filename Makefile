CXX 	  := g++
CXXFLAGS := -std=c++17 -Wall -Wno-format-security
LDFLAGS   := -L/usr/lib -lstdc++ -lm -ldl -lncurses -ltinfo
SRC 	  := $(wildcard examples/*.cpp)
TARGETS   := $(SRC:examples/%.cpp=%)

all: $(TARGETS) clean

%: examples/%.cpp
	@clear
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) -o bin/$@ $< $(LDFLAGS)
	./bin/$@

clean:
	@rm -f bin/*
