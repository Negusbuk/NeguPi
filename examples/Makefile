EXAMPLES= blink \
          statemachine \
          timelapse \
          videocapture \
          camerabox

INCPATHS=../src
LIBPATHS=../src

MACHINE:= $(shell uname -m)
NODEVICE=0
ifneq "$(MACHINE)" "armv6l"
	NODEVICE=1
endif

LDFLAGS=-L../src -lNeguPi
CPPFLAGS=-std=c++0x -Wall
CPP=g++

ifeq "$(NODEVICE)" "1"
CPPFLAGS+= -DNODEVICE
endif

# ------------ MAGIC BEGINS HERE -------------

# Automatic generation of some important lists
OBJECTS=$(SOURCES:.cc=.o)
INCFLAGS=$(foreach TMP,$(INCPATHS),-I$(TMP))
LIBFLAGS=$(foreach TMP,$(LIBPATHS),-L$(TMP))

all: $(EXAMPLES)

blink: blink.cc ../src/libNeguPi.a
	g++ -o $@ $< $(CPPFLAGS) $(INCFLAGS) $(LDFLAGS)

statemachine: statemachine.cc ../src/libNeguPi.a
	g++ -o $@ $< $(CPPFLAGS) $(INCFLAGS) $(LDFLAGS)

timelapse: timelapse.cc ../src/libNeguPi.a
	g++ -o $@ $< $(CPPFLAGS) $(INCFLAGS) $(LDFLAGS)

videocapture: videocapture.cc ../src/libNeguPi.a
	g++ -o $@ $< $(CPPFLAGS) $(INCFLAGS) $(LDFLAGS)

camerabox: camerabox.cc ../src/libNeguPi.a
	g++ -o $@ $< $(CPPFLAGS) $(INCFLAGS) $(LDFLAGS)
	
distclean: clean
	rm -fr ../bin

clean:
	rm -f $(EXAMPLES)

install: all
	@(test -e ~/bin || mkdir ~/bin)
	cp $(EXAMPLES) ~/bin
	cp videocapture.py ~/bin
