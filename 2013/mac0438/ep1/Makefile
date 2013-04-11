CC = gcc
OPTS = -Wall -pedantic -ansi -O2 -U_FORTFY_SOURCE -g

ep1: ep1.o tempos.o defines.o
	$(CC) ep1.o tempos.o defines.o -o ep1 -lm -lpthread

ep1.o: ep1.c defines.h tempos.h
	$(CC) -c $(OPTS) ep1.c defines.h 

tempos.o: tempos.c defines.h tempos.h
	$(CC) -c $(OPTS) tempos.c defines.h tempos.h

defines.o: defines.c defines.h
	$(CC) -c $(OPTS) defines.c defines.h

clean:
	rm *.o
	rm *.gch
	rm ep1
