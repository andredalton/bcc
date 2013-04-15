CC = gcc
OPTS = -Wall -pedantic -ansi -O2 -U_FORTFY_SOURCE -g

# Verifica as dependencias para o binário principal
ep1: ep1.o tempos.o defines.o
	touch tmp.h
	$(CC) ep1.o tempos.o defines.o -o $@ -lm -lpthread
	rm tmp.h


ep1.o: ep1.c defines.h tempos.h
	touch tmp.h
	$(CC) -c $(OPTS) ep1.c defines.h 
	rm tmp.h

tempos.o: tempos.c defines.h tempos.h
	touch tmp.h
	$(CC) -c $(OPTS) tempos.c defines.h tempos.h
	rm tmp.h

defines.o: defines.c defines.h
	touch tmp.h
	$(CC) -c $(OPTS) defines.c defines.h
	rm tmp.h

# Versões extras:

# Binário que simula o algoritmo da punição. 
simula: defines.c defines.h
	echo "#define SIMULACAO 1" > tmp.h
	echo "#define PRECISAO 1" >> tmp.h
	$(CC) $(OPTS) defines.c -o $@
	rm tmp.h
	echo "\n\n\n"
	./simula -h

clean:
	rm -f *.o
	rm -f *.gch
	rm -f ep1
	rm -f ep1-sleep
	rm -f simula

# Binário para depuração. Espera 0.1s para imprimir a próxima linha da simulação.
ep1-sleep:
	echo "#define TIME 100000" > tmp.h
	$(CC) -c $(OPTS) ep1.c defines.h
	$(CC) ep1.o tempos.o defines.o -o ep1-sleep -lm -lpthread
	rm tmp.h

ep1-mili:
	echo "#define PRECISAO 1000" > tmp.h
	$(CC) -c $(OPTS) tempos.c defines.h tempos.h
	$(CC) -c $(OPTS) tempos.c defines.h tempos.h
	$(CC) -c $(OPTS) ep1.c defines.h
	$(CC) ep1.o tempos.o defines.o -o ep1-mili -lm -lpthread
	rm tmp.h

all:
	touch tmp.h
	$(CC) -c $(OPTS) ep1.c defines.h
	$(CC) -c $(OPTS) tempos.c defines.h tempos.h
	$(CC) -c $(OPTS) defines.c defines.h
	$(CC) ep1.o tempos.o defines.o -o ep1 -lm -lpthread
	echo "#define TIME 100000" > tmp.h
	$(CC) -c $(OPTS) ep1.c defines.h
	$(CC) ep1.o tempos.o defines.o -o ep1-sleep -lm -lpthread
	rm tmp.h