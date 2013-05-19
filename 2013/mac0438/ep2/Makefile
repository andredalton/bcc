#/**********
# * Makefile
# *********/
#/******************************************************************************
# * Alunos: André Meneghelli Vale,      Núm. USP: 4898948
# *         Marcello Souza de Oliveira, Núm. USP: 6432692
# * Curso: Bacharelado em Ciências da Computação
# * EP 02 -- Calculo do valor aproximado de PI.
# * MAC0438 -- 17/04/2013 -- IME/USP, -- Prof. Daniel Macêdo Batista
# * Compilador: gcc linux 4.6.3
# * Editor: Sublime Text 2;
# * Sistema Operacional: Linux
# *****************************************************************************/

CC = gcc
CCOPTS = -Wall -pedantic -ansi -O2 -U_FORTFY_SOURCE -g
LDOPTS = -lm -lpthread

# Verifica as dependencias para o binário principal
ep2: ep2.c
	@$(CC) -c $(CCOPTS) ep2.c
	@$(CC) ep2.o -o $@ $(LDOPTS)

# Binário que simula o algoritmo da punição.
#simula: defines.c defines.h
#	@$(CC) $(OPTS) -DSIMULACAO=1 -DPRECISAO=1 defines.c -o $@

# limpa o diretório de compilação.
clean:
	@echo Limpando diretorio
	@rm -f *.o
	@rm -f *.gch
	@rm -f *.out
	@rm -f ep2
	@rm -f relatorios/relatorio.aux
	@rm -f relatorios/relatorio.log
	@rm -f relatorios/relatorio.pdf
	@rm -f relatorios/relatorio.out
	@rm -f relatorios/mycpu.tex
	@rm -f relatorios/myep1.tex
	@rm -f relatorios/maquinas.tex
	@rm -f relatorios/tempos.tex

# Gera todos os arquivos binários.
all: ep2