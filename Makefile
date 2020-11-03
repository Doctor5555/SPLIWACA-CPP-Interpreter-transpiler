CXX = clang++
CC = gcc

CXXFLAGS := -std=c++1z -I Spliwaca/Spliwaca/src/ -I Spliwaca/vendor/spdlog/include --define-macro SPLW_DEBUG
CFLAGS := -std=c11
LDFLAGS := 

OBJ_PREFIX = Spliwaca/Spliwaca/src/
OBJS := $(OBJ_PREFIX)Main.o $(OBJ_PREFIX)Transpiler.o $(OBJ_PREFIX)Log.o $(OBJ_PREFIX)Frontend/Lexer/Lexer.o $(OBJ_PREFIX)Frontend/Parser/IdentNode.o $(OBJ_PREFIX)Frontend/Parser/Parser.o $(OBJ_PREFIX)Backend/Generation/Python/Generator.o $(OBJ_PREFIX)Backend/Generation/LLVM/Generator.o

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: test build clean

all: build

build: $(OBJS)
	mkdir -p build
	$(CC) $(LDFLAGS) $(OBJS) -o transpiler

release: CFLAGS +=
release: CXXFLAGS += -O3
release: $(OBJS)
release: build

test: build
	./build/rwc.out

clean:
	rm -rf src/*.o
	rm -rf src/*.d
	rm -rf srcpp/*.o
	rm -rf srcpp/*.d
	rm -rf build