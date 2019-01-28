TOOLS=exposurecomp

INCPATHS=../src ./
LIBPATHS=../src

MACHINE:= $(shell uname -m)
NODEVICE=0
ifneq "$(MACHINE)" "armv7l"
	NODEVICE=1
	INCPATHS+=/opt/local/include /opt/local/include/opencv /opt/local/include/opencv2
	LDFLAGS=-L/opt/local/lib -lconfig++
else
	LDFLAGS=-lconfig++
endif

LDFLAGS+=-L../src -lNeguPi
CPPFLAGS+=-std=c++0x -Wall
CPP=g++

ifeq "$(NODEVICE)" "1"
CPPFLAGS+= -DNODEVICE
else
CPPFLAGS+= $(shell pkg-config --cflags opencv) -I/usr/include/opencv2
endif

LDFLAGS+= $(shell pkg-config --libs opencv)

# ------------ MAGIC BEGINS HERE -------------

# Automatic generation of some important lists
OBJECTS=$(SOURCES:.cc=.o)
INCFLAGS=$(foreach TMP,$(INCPATHS),-I$(TMP))
LIBFLAGS=$(foreach TMP,$(LIBPATHS),-L$(TMP))

all: $(TOOLS)

exposurecomp: exposurecomp.cc
	g++ -o $@ $< $(CPPFLAGS) $(INCFLAGS) $(LDFLAGS)

distclean: clean
	rm -fr ../bin

clean:
	rm -f $(EXAMPLES)

install: all
	@(test -e ~/bin || mkdir ~/bin)
	cp $(TOOLS) ~/bin
