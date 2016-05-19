
CC = g++

IDIR = -I./flatbuffers

DEFS = -D_FILE_OFFSET_BITS=64 -D_GNU_SOURCE 
CFLAGS = -std=c++0x -O0 -g $(IDIR) $(EXTLIB_INCL) $(DEFS)

LDFLAGS = -L ./

LIBOBJECT = -lpthread

TARGET= tv_test

#SRCS := $(shell find ./src -name '*.cpp')
#SRCS := $(sort $(SRCS))
#OBJECTS += $(SRCS:%.cpp=%.o)
SRCS = $(wildcard *.cpp)
OBJECTS := $(patsubst %cpp,%o,$(SRCS))

all: $(TARGET)

%.o:%.cpp  
	$(CC) $(CFLAGS) $(LDFLAGS) -c $<


$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBOBJECT)

clean:
	@rm -f *.o *~ core $(TARGET)
