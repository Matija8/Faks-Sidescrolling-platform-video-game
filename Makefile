PROGRAM = Game.out
CC      = gcc
CFLAGS  = -g -Wall -I/usr/X11R6/include -I/usr/pkg/include
LDFLAGS = -L/usr/X11R6/lib -L/usr/pkg/lib
LDLIBS  = -lglut -lGLU -lGL

$(PROGRAM): game.o image.o functions.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(PROGRAM) image.o game.o functions.o $(LDLIBS)
	rm functions.o image.o game.o

game.o: game.c
	$(CC) $(CFLAGS) game.c -c $(LDLIBS)

image.o: image.c
	$(CC) $(CFLAGS) image.c -c $(LDLIBS)

functions.o: functions.c
	$(CC) $(CFLAGS) functions.c -c $(LDLIBS)


