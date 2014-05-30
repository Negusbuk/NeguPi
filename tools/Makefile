TOOLS=exposurecomp

INCPATHS=../src ./
LIBPATHS=../src

MACHINE:= $(shell uname -m)
NODEVICE=0
ifneq "$(MACHINE)" "armv6l"
	NODEVICE=1
	INCPATHS+=/opt/local/include /opt/local/include/opencv /opt/local/include/opencv2
	LDFLAGS=-L/opt/local/lib -lconfig++ -lopencv_core -lopencv_imgproc -lopencv_highgui
	LDFLAGS+=-lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect
	LDFLAGS+=-lopencv_stitching -lopencv_contrib -lopencv_legacy -lopencv_flann
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
LDFLAGS+= $(shell pkg-config --libs opencv)
endif

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
