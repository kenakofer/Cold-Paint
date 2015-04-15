CC=gcc
CFLAGS=-std=c99 -lSDL2 -lm -g

cold_paint: cold_paint.c objects.o objectlist.o events.o graphics.o bitoperations.o
	$(CC) -o cold_paint cold_paint.c objects.o objectlist.o events.o graphics.o bitoperations.o $(CFLAGS)
objects.o: objects.c objectlist.o events.o graphics.o bitoperations.o
	$(CC) -c objects.c $(CFLAGS)
objectlist.o: objectlist.c
	$(CC) -c objectlist.c $(CFLAGS)
events.o: events.c graphics.o
	$(CC) -c events.c $(CFLAGS)
graphics.o: graphics.c
	$(CC) -c graphics.c $(CFLAGS)
bitoperations.o: bitoperations.c
	$(CC) -c bitoperations.c $(CFLAGS)

