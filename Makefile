PROJECT = NeguPi

SOURCES = src/VMCP23S17.cc \
		  src/MCP23S17Fake.cc \
		  src/MCP23S17.cc \
		  src/VPiFace.cc \
		  src/PiFace.cc \
		  src/PiFaceFake.cc \
          src/PiFaceDaemon.cc \
		  src/PiFaceStateMachine.cc \
          src/PiFaceLogger.cc

LIBRARY = static

INCPATHS=../libmcp23s17/src/ ./src/
LIBPATHS=../libmcp23s17/

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

# ------------ MAGIC BEGINS HERE -------------

# Automatic generation of some important lists
OBJECTS=$(SOURCES:.cc=.o)
INCFLAGS=$(foreach TMP,$(INCPATHS),-I$(TMP))
LIBFLAGS=$(foreach TMP,$(LIBPATHS),-L$(TMP))

# Set up the output file names for the different output types
ifeq "$(LIBRARY)" "shared"
    BINARY=lib$(PROJECT).so
    LDFLAGS += -shared
else ifeq "$(LIBRARY)" "static"
    BINARY=lib$(PROJECT).a
else
    BINARY=$(PROJECT)
endif

all: $(SOURCES) $(BINARY)
	make -C examples

$(BINARY): $(OBJECTS)
    # Link the object files, or archive into a static library
    ifeq "$(LIBRARY)" "static"
	ar rcs $(BINARY) $(OBJECTS)
    else
	$(CPP) $(LIBFLAGS) $(OBJECTS) $(LDFLAGS) -o $@
    endif

.cc.o:
	$(CPP) $(INCFLAGS) $(CPPFLAGS) -fPIC $< -o $@

distclean: clean
	rm -f $(BINARY)

clean:
	rm -f $(OBJECTS)
	make -C examples clean
	
