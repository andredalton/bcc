CC = gcc
OPTS = -Wall -D _BSD_SOURCE -pedantic -ansi -O2 -U_FORTFY_SOURCE -g

# Verifica as dependencias para o binário principal
ep1: ep1.c defines.c defines.h tempos.c tempos.h
	$(CC) -c $(OPTS) defines.c defines.h
	$(CC) -c $(OPTS) tempos.c defines.h tempos.h
	$(CC) -c $(OPTS) ep1.c defines.h 
	$(CC) ep1.o tempos.o defines.o -o $@ -lm -lpthread

# Versões extras:
# Binário que simula o algoritmo da punição. 
simula: defines.c defines.h
	$(CC) $(OPTS) -DSIMULACAO=1 -DPRECISAO=1 defines.c -o $@
	@./simula -h

# Binário para depuração. Espera 0.1s para imprimir a próxima linha da simulação.
ep1-sleep: ep1.c defines.c defines.h tempos.c tempos.h
	$(CC) -c $(OPTS) -DTIME=100000 tempos.c defines.h tempos.h
	$(CC) -c $(OPTS) -DTIME=100000 defines.c defines.h
	$(CC) -c $(OPTS) -DTIME=100000 ep1.c defines.h
	$(CC) ep1.o tempos.o defines.o -o $@ -lm -lpthread

# Binário que usa precisão de milisegundos.
ep1-mili: ep1.c defines.c defines.h tempos.c tempos.h
	$(CC) -c $(OPTS) -DPRECISAO=1000 tempos.c defines.h tempos.h
	$(CC) -c $(OPTS) -DPRECISAO=1000 defines.c defines.h
	$(CC) -c $(OPTS) -DPRECISAO=1000 ep1.c defines.h
	$(CC) ep1.o tempos.o defines.o -o $@ -lm -lpthread

# Binário que roda sem as sessões criticas. (Sem punições)
# em segundos:
pep1: ep1.c defines.c defines.h tempos.c tempos.h
	$(CC) -c $(OPTS) -DPARALELO=1 tempos.c defines.h tempos.h
	$(CC) -c $(OPTS) -DPARALELO=1 defines.c defines.h
	$(CC) -c $(OPTS) -DPARALELO=1 ep1.c defines.h
	$(CC) ep1.o tempos.o defines.o -o pep1 -lm -lpthread
# em milisegundos.
pep1-mili: ep1.c defines.c defines.h tempos.c tempos.h
	$(CC) -c $(OPTS) -DPARALELO=1 -DPRECISAO=1000 tempos.c defines.h tempos.h
	$(CC) -c $(OPTS) -DPARALELO=1 -DPRECISAO=1000 defines.c defines.h
	$(CC) -c $(OPTS) -DPARALELO=1 -DPRECISAO=1000 ep1.c defines.h
	$(CC) ep1.o tempos.o defines.o -o pep1-mili -lm -lpthread

# limpa o diretório.
clean:
	@echo Limpando diretorio
	@rm -f *.o
	@rm -f *.gch
	@rm -f ep1
	@rm -f pep1
	@rm -f ep1-sleep
	@rm -f ep1-mili
	@rm -f pep1-mili
	@rm -f simula
	@rm -f relatorio.txt