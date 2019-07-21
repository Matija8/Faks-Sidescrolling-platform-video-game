PROGRAM = 1
CC      = gcc
CFLAGS  = -g -Wall -I/usr/X11R6/include -I/usr/pkg/include
LDFLAGS = -L/usr/X11R6/lib -L/usr/pkg/lib
LDLIBS  = -lglut -lGLU -lGL

$(PROGRAM): 1.o image.o functions.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(PROGRAM) image.o 1.o functions.o $(LDLIBS)
	rm functions.o image.o 1.o

1.o: 1.c
	$(CC) $(CFLAGS) 1.c -c $(LDLIBS)

image.o: image.c
	$(CC) $(CFLAGS) image.c -c $(LDLIBS)

functions.o: functions.c
	$(CC) $(CFLAGS) functions.c -c $(LDLIBS)


