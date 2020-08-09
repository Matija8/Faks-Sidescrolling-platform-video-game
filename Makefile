PROGRAM = a.out
CC = gcc
WFLAGS 	= -Wall -Wextra
HINCLUDES = -I/usr/X11R6/include -I/usr/pkg/include
CFLAGS 	= $(WFLAGS) $(HINCLUDES)
LDFLAGS = -L/usr/X11R6/lib -L/usr/pkg/lib
LDLIBS 	= -lglut -lGLU -lGL

$(PROGRAM): game.o image.o functions.o
	$(CC) $(HINCLUDES) $(LDFLAGS) -o $(PROGRAM) image.o game.o functions.o $(LDLIBS)

game.o: game.c
	$(CC) $(CFLAGS) game.c -c $(LDLIBS)

image.o: image.c
	$(CC) $(CFLAGS) image.c -c $(LDLIBS)

functions.o: functions.c
	$(CC) $(CFLAGS) functions.c -c $(LDLIBS)

.PHONY: clean

clean:
	rm -f *.o $(PROGRAM)

