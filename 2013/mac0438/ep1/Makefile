#/**********
# * Makefile
# *********/
#/******************************************************************************
# * Alunos: André Meneghelli Vale,      Núm. USP: 4898948
# *         Marcello Souza de Oliveira, Núm. USP: 6432692
# * Curso: Bacharelado em Ciências da Computação
# * EP 01 -- Simulação Concorrente de uma prova de 'Iron Man'
# * MAC0438 -- 17/04/2013 -- IME/USP, -- Prof. Daniel Macêdo Batista
# * Compilador: gcc linux 4.6.3
# * Editor: Sublime Text 2;
# * Sistema Operacional: Linux
# *****************************************************************************/

CC = gcc
OPTS = -Wall -D _BSD_SOURCE -pedantic -ansi -O2 -U_FORTFY_SOURCE -g

# Verifica as dependencias para o binário principal
ep1: ep1.c defines.c defines.h tempos.c tempos.h
	@$(CC) -c $(OPTS) defines.c defines.h
	@$(CC) -c $(OPTS) tempos.c defines.h tempos.h
	@$(CC) -c $(OPTS) ep1.c defines.h 
	@$(CC) ep1.o tempos.o defines.o -o $@ -lm -lpthread

# Versões extras:
# Binário que usa precisão de milisegundos.
ep1-mili: ep1.c defines.c defines.h tempos.c tempos.h
	@$(CC) -c $(OPTS) -DPRECISAO=1000 tempos.c defines.h tempos.h
	@$(CC) -c $(OPTS) -DPRECISAO=1000 defines.c defines.h
	@$(CC) -c $(OPTS) -DPRECISAO=1000 ep1.c defines.h
	@$(CC) ep1.o tempos.o defines.o -o $@ -lm -lpthread

# Binário que roda sem as sessões criticas. (Sem punições)
# em segundos:
pep1: ep1.c defines.c defines.h tempos.c tempos.h
	@$(CC) -c $(OPTS) -DPARALELO=1 tempos.c defines.h tempos.h
	@$(CC) -c $(OPTS) -DPARALELO=1 defines.c defines.h
	@$(CC) -c $(OPTS) -DPARALELO=1 ep1.c defines.h
	@$(CC) ep1.o tempos.o defines.o -o $@ -lm -lpthread

# em milisegundos.
pep1-mili: ep1.c defines.c defines.h tempos.c tempos.h
	@$(CC) -c $(OPTS) -DPARALELO=1 -DPRECISAO=1000 tempos.c defines.h tempos.h
	@$(CC) -c $(OPTS) -DPARALELO=1 -DPRECISAO=1000 defines.c defines.h
	@$(CC) -c $(OPTS) -DPARALELO=1 -DPRECISAO=1000 ep1.c defines.h
	@$(CC) ep1.o tempos.o defines.o -o $@ -lm -lpthread

# Binário que usa uma função principal sem paralelismo. 
gep1: ep1.c defines.c defines.h tempos.c tempos.h
	@$(CC) -c $(OPTS) -DITERATIVO=1 tempos.c defines.h tempos.h
	@$(CC) -c $(OPTS) -DITERATIVO=1 defines.c defines.h
	@$(CC) -c $(OPTS) -DITERATIVO=1 ep1.c defines.h
	@$(CC) ep1.o tempos.o defines.o -o $@ -lm -lpthread

# em milisegundos.
gep1-mili: ep1.c defines.c defines.h tempos.c tempos.h
	@$(CC) -c $(OPTS) -DITERATIVO=1 -DPRECISAO=1000 tempos.c defines.h tempos.h
	@$(CC) -c $(OPTS) -DITERATIVO=1 -DPRECISAO=1000 defines.c defines.h
	@$(CC) -c $(OPTS) -DITERATIVO=1 -DPRECISAO=1000 ep1.c defines.h
	@$(CC) ep1.o tempos.o defines.o -o $@ -lm -lpthread

# Binário para depuração. Espera 0.1s para imprimir a próxima linha da simulação.
ep1-sleep: ep1.c defines.c defines.h tempos.c tempos.h
	@$(CC) -c $(OPTS) -DTIME=100000 tempos.c defines.h tempos.h
	@$(CC) -c $(OPTS) -DTIME=100000 defines.c defines.h
	@$(CC) -c $(OPTS) -DTIME=100000 ep1.c defines.h
	@$(CC) ep1.o tempos.o defines.o -o $@ -lm -lpthread

# Binário que simula o algoritmo da punição. 
simula: defines.c defines.h
	@$(CC) $(OPTS) -DSIMULACAO=1 -DPRECISAO=1 defines.c -o $@

# limpa o diretório de compilação.
clean:
	@echo Limpando diretorio
	@rm -f *.o
	@rm -f *.gch
	@rm -f ep1
	@rm -f pep1
	@rm -f gep1
	@rm -f ep1-mili
	@rm -f pep1-mili
	@rm -f gep1-mili
	@rm -f ep1-sleep
	@rm -f simula
	@rm -f relatorios/relatorio.aux
	@rm -f relatorios/relatorio.log
	@rm -f relatorios/relatorio.pdf
	@rm -f relatorios/relatorio.out
	@rm -f relatorios/mycpu.tex
	@rm -f relatorios/myep1.tex
	@rm -f relatorios/maquinas.tex
	@rm -f relatorios/tempos.tex

# Gera todos os arquivos binários.
all: ep1 pep1 gep1 ep1-mili pep1-mili gep1-mili ep1-sleep simula