CC = g++
CPPFLAGS = -std=gnu++0x -g -Wall -Wno-unused-function `pkg-config opencv --cflags`
LDFLAGS = `pkg-config opencv --libs`

SOURCES = opencvtest.cpp colorSegment.cpp timetest.cpp camera_test.cpp

TARG = $(SOURCES:.cpp=)

all: $(TARG)

%: %.cpp
	$(CC) $(CPPFLAGS) $? -o $@ $(LDFLAGS)

clean:
	rm -f *~ *.o $(TARG)
