PROGRAM = a.out
CXX = g++
WFLAGS 	= -Wall -Wextra -std=c++17
HINCLUDES = -I/usr/X11R6/include -I/usr/pkg/include
CFLAGS 	= $(WFLAGS) $(HINCLUDES)
LDFLAGS = -L/usr/X11R6/lib -L/usr/pkg/lib
LDLIBS 	= -lglut -lGLU -lGL

MAKE_OBJECT_FILE = $(CXX) $(CFLAGS) $< -c $(LDLIBS)

$(PROGRAM): game.o level.o functions.o image.o player.o textures.o window.o
	$(CXX) $(HINCLUDES) $(LDFLAGS) $^ $(LDLIBS) -o $(PROGRAM)

game.o: game.cpp constants.h window.h error_handling.h
	$(MAKE_OBJECT_FILE)

level.o: level.cpp level.h
	$(MAKE_OBJECT_FILE)

functions.o: functions.cpp functions.h
	$(MAKE_OBJECT_FILE)

image.o: image.c image.h
	$(MAKE_OBJECT_FILE)

player.o: player.cpp player.h
	$(MAKE_OBJECT_FILE)

textures.o: textures.cpp textures.h
	$(MAKE_OBJECT_FILE)

window.o: window.cpp window.h
	$(MAKE_OBJECT_FILE)

.PHONY: clean

clean:
	rm -f *.o $(PROGRAM)

