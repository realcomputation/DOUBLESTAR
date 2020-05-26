CC = gcc -std=c11
CFLAGS = -g -O2
CPP = gcc -E
CPPFLAGS =     -I./include
CXX = g++ -std=c++11
CXXCPP = g++ -E -std=c++11
CXXFLAGS = -g -O2
LDFLAGS = -Xlinker -rpath -Xlinker ./lib
LDLIBS =  -L./lib -lmpfr -lgmp -lm -lpthread -lqd

BIN = bench_mpfr_dd_qd

all: $(BIN)

maintainer-clean: distclean
distclean: clean
	rm -f Makefile

clean:
	rm -f $(BIN)

install:
