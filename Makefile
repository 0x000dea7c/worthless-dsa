CXX      := g++
CXXFLAGS := -Wall -std=c++17 -ggdb -fsanitize=address,leak,undefined
SOURCES  := $(wildcard src/*.cc)
TARGETS  := $(patsubst src/%.cc, %, $(SOURCES))

all: $(TARGETS)

%: src/%.cc
	$(CXX) $(CXXFLAGS) $< -o $@

.PHONY: all clean

clean:
	rm -f $(TARGETS)
