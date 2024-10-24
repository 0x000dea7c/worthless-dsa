CXX      := g++
CXXFLAGS := -Wall -std=gnu++23 -ggdb -fsanitize=address,leak,undefined
SOURCES  := $(wildcard src/*.cc)
TARGETS  := $(patsubst src/%.cc, %, $(SOURCES))

all: $(TARGETS)

%: src/%.cc
	$(CXX) $(CXXFLAGS) $< -o $@

.PHONY: all clean

clean:
	rm -f $(TARGETS)
