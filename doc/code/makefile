# iRRAM path [/usr/local]
iRRAM_path=/usr/local



prefix=$(iRRAM_path)
exec_prefix=$(prefix)
CC = gcc -std=c11
CFLAGS = -g -O2
CPP = gcc -E
CPPFLAGS =     -I$(prefix)/include
CXX = g++ -std=c++11
CXXCPP = g++ -E -std=c++11
CXXFLAGS = -g -O2
LDFLAGS = -Xlinker -rpath -Xlinker $(prefix)/lib
LDLIBS =  -L$(prefix)/lib -liRRAM -lmpfr -lgmp -lm -lpthread -lqd

EXAMPLES_BIN = \
		1 2-1 2-2 2-3 2-4 2-5

all: $(EXAMPLES_BIN)

maintainer-clean: distclean
distclean: clean
	rm -f Makefile

clean:
	rm -f $(EXAMPLES_BIN)

install: