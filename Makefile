# Enable multi-threading
MAKEFLAGS += -j4

# Compiler flags
CXX 	  := g++-13
CXXFLAGS := -std=c++20 -Wall -Wno-format-security -Wno-sign-compare -pedantic-errors
LDFLAGS   := -L/usr/lib -lstdc++ -lm -ldl -ltinfo -lbfd -ldw

# Directories
SRC 	  := $(wildcard examples/*.cpp)
TARGETS   := $(SRC:examples/%.cpp=%)

# Build type (debug, release)
BUILD_TYPE := debug
ifeq ($(BUILD_TYPE),debug)
    CXXFLAGS += -O0 -DDEBUG -g
else ifeq ($(BUILD_TYPE),release)
    CXXFLAGS += -O3 -DNDEBUG
endif

# Commands

.PHONY: build
all: $(TARGETS) test clean

test: tests/tests.cpp
	@clear
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) -Ivendor -o bin/$@ $< $(LDFLAGS)
	./bin/$@

%: examples/%.cpp
	@clear
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) -Ivendor -o bin/$@ $< $(LDFLAGS)
	./bin/$@

clean:
	@rm -f bin/*
