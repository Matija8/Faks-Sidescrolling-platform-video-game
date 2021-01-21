PROGRAM = a.out
CXX = g++
WFLAGS 	= -Wall -Wextra
HINCLUDES = -I/usr/X11R6/include -I/usr/pkg/include
CFLAGS 	= $(WFLAGS) $(HINCLUDES)
LDFLAGS = -L/usr/X11R6/lib -L/usr/pkg/lib
LDLIBS 	= -lglut -lGLU -lGL

$(PROGRAM): game.o image.o functions.o
	$(CXX) $(HINCLUDES) $(LDFLAGS) -o $(PROGRAM) image.o game.o functions.o $(LDLIBS)

game.o: game.cpp
	$(CXX) $(CFLAGS) $< -c $(LDLIBS)

image.o: image.c
	$(CXX) $(CFLAGS) $< -c $(LDLIBS)

functions.o: functions.cpp
	$(CXX) $(CFLAGS) $< -c $(LDLIBS)

.PHONY: clean

clean:
	rm -f *.o $(PROGRAM)

