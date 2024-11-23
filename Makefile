CXX      := g++
CXXFLAGS := -Wall -std=c++17 -g -O3 -march=native -mavx2 -fsanitize=address,leak,undefined -ftrapv -Wstrict-overflow
# -fsanitize=address,leak,undefined
# ftrapv: makes program abort if integer overflow happenz, but honestly the sanitiser should do it
# Wstrict-overflow: warns about optimisations that assume signed overflow cannot happen
SOURCES  := $(wildcard src/*.cc)
TARGETS  := $(patsubst src/%.cc, %, $(SOURCES))

all: $(TARGETS)

%: src/%.cc
	$(CXX) $(CXXFLAGS) $< -o $@

.PHONY: all clean

clean:
	rm -f $(TARGETS)
