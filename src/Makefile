
SOURCES = NeguPiLogger.cc \
          NeguPiDaemon.cc \
          VMCP23S17.cc \
	  MCP23S17Fake.cc \
	  MCP23S17.cc \
	  PiFace.cc \
	  PiFaceStateMachine.cc

MACHINE:= $(shell uname -m)
NODEVICE=0
ifneq "$(MACHINE)" "armv6l"
	NODEVICE=1
endif

LDFLAGS=
CPPFLAGS=-std=c++0x -c -Wall
CPP=g++

ifeq "$(NODEVICE)" "1"
CPPFLAGS+= -DNODEVICE
endif

INCPATHS=.
LIBPATHS=

# ------------ MAGIC BEGINS HERE -------------

# Automatic generation of some important lists
OBJECTS=$(SOURCES:.cc=.o)
HEADERS=$(SOURCES:.cc=.h)
INCFLAGS=$(foreach TMP,$(INCPATHS),-I$(TMP))
LIBFLAGS=$(foreach TMP,$(LIBPATHS),-L$(TMP))

all: libNeguPi.a

libNeguPi.a: $(OBJECTS)
	ar rcs libNeguPi.a $(OBJECTS)

.cc.o:
	$(CPP) $(INCFLAGS) $(CPPFLAGS) -fPIC $< -o $@

install:
	mkdir ../include
	cp $(HEADERS) ../include
	mkdir ../lib
	cp libNeguPi.a ../lib
	
distclean: clean
	rm -fr ../include
	rm -fr ../lib

clean:
	rm -f $(OBJECTS)
	
