EXAMPLES= blink \
          statemachine \
          timelapse \
          videocapture \
          camerabox \
          recognition

INCPATHS=../src ./
LIBPATHS=../src

MACHINE:= $(shell uname -m)
NODEVICE=0
ifneq "$(MACHINE)" "armv6l"
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
CPPFLAGS+= $(shell pkg-config --cflags opencv)
endif

LDFLAGS+= $(shell pkg-config --libs opencv)

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

timelapse: timelapse.cc imageoptions.cc imageoptions.h ../src/libNeguPi.a
	g++ -o $@ timelapse.cc imageoptions.cc $(CPPFLAGS) $(INCFLAGS) $(LDFLAGS)

videocapture: videocapture.cc imageoptions.cc imageoptions.h ../src/libNeguPi.a
	g++ -o $@ videocapture.cc imageoptions.cc $(CPPFLAGS) $(INCFLAGS) $(LDFLAGS)

camerabox: camerabox.cc camerabox.h imageoptions.cc imageoptions.h ../src/libNeguPi.a
	g++ -o $@ camerabox.cc imageoptions.cc $(CPPFLAGS) $(INCFLAGS) $(LDFLAGS)

recognition: recognition.cc ../src/libNeguPi.a
	g++ -o $@ $< $(CPPFLAGS) $(INCFLAGS) $(LDFLAGS)

distclean: clean
	rm -fr ../bin

clean:
	rm -f $(EXAMPLES)

install: all
	@(test -e ~/bin || mkdir ~/bin)
	cp $(EXAMPLES) ~/bin
	cp videocapture.py ~/bin
