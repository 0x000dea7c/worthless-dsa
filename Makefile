ASM      := nasm
ASMFLAGS := -f elf64 -g
LD       := ld
CXX      := g++
CXXFLAGS := -Wall -std=c++17 -g -O3 -march=native -mavx2  -ftrapv -Wstrict-overflow -fsanitize=address,leak,undefined
CC       := cc
CFLAGS := -Wall -std=c99 -g -O3 -march=native -mavx2  -ftrapv -Wstrict-overflow -fsanitize=address,leak,undefined

#
# ftrapv: makes program abort if integer overflow happenz, but honestly the sanitiser should do it
# Wstrict-overflow: warns about optimisations that assume signed overflow cannot happen
SOURCES     := $(wildcard src/*.cc)
TARGETS     := $(patsubst src/%.cc, %, $(SOURCES))

ASM_SOURCES := $(wildcard src/*.asm)
ASM_TARGETS := $(patsubst src/%.asm, %, $(ASM_SOURCES))

C_SOURCES   := $(wildcard src/*.c)
C_TARGETS   := $(patsubst src/%.c, %, $(C_SOURCES))

all: $(TARGETS) $(ASM_TARGETS)

%: src/%.cc
	$(CXX) $(CXXFLAGS) $< -o $@

%: src/%.asm
	$(ASM) $(ASMFLAGS) $< -o $@.o
	$(LD) $@.o -o $@
	rm $@.o

%: src/%.c
	$(CXX) $(CXXFLAGS) $< -o $@

.PHONY: all clean

clean:
	rm -f $(TARGETS) $(ASM_TARGETS) $(C_TARGETS)
