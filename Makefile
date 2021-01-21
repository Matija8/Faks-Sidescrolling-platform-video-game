PROGRAM = a.out
CXX = g++
WFLAGS 	= -Wall -Wextra -std=c++17
HINCLUDES = -I/usr/X11R6/include -I/usr/pkg/include
CFLAGS 	= $(WFLAGS) $(HINCLUDES)
LDFLAGS = -L/usr/X11R6/lib -L/usr/pkg/lib
LDLIBS 	= -lglut -lGLU -lGL

$(PROGRAM): game.o image.o functions.o window.o
	$(CXX) $(HINCLUDES) $(LDFLAGS) $^ $(LDLIBS) -o $(PROGRAM)

game.o: game.cpp constants.h window.h error_handling.h
	$(CXX) $(CFLAGS) $< -c $(LDLIBS)

image.o: image.c
	$(CXX) $(CFLAGS) $< -c $(LDLIBS)

functions.o: functions.cpp
	$(CXX) $(CFLAGS) $< -c $(LDLIBS)

window.o: window.cpp window.h
	$(CXX) $(CFLAGS) $< -c $(LDLIBS)

.PHONY: clean

clean:
	rm -f *.o $(PROGRAM)

