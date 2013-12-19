EXAMPLES=blink statemachine

INCPATHS=../../libmcp23s17/src/ ../src
LIBPATHS=../../libmcp23s17/ ../

MACHINE:= $(shell uname -m)
NODEVICE=0
ifneq "$(MACHINE)" "armv6l"
	NODEVICE=1
endif

LDFLAGS=-L../ -lNeguPi
CPPFLAGS=-std=c++0x -Wall
CPP=g++

ifeq "$(NODEVICE)" "1"
CPPFLAGS+= -DNODEVICE
else
LDFLAGS+= -L../../libmcp23s17/ -lmcp23s17
endif

# ------------ MAGIC BEGINS HERE -------------

# Automatic generation of some important lists
OBJECTS=$(SOURCES:.cc=.o)
INCFLAGS=$(foreach TMP,$(INCPATHS),-I$(TMP))
LIBFLAGS=$(foreach TMP,$(LIBPATHS),-L$(TMP))

all: $(EXAMPLES)

blink: blink.cc
	g++ -o $@ $< $(CPPFLAGS) $(INCFLAGS) $(LDFLAGS)

statemachine: statemachine.cc
	g++ -o $@ $< $(CPPFLAGS) $(INCFLAGS) $(LDFLAGS)

clean:
	rm -f $(EXAMPLES)
