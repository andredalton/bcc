/***********
 * defines.h
 **********/
/*******************************************************************************
 * Alunos: André Meneghelli Vale,      Núm. USP: 4898948
 *         Marcello Souza de Oliveira, Núm. USP: 6432692
 * Curso: Bacharelado em Ciências da Computação
 * EP 01 -- Simulação Concorrente de uma prova de 'Iron Man'
 * MAC0438 -- 17/04/2013 -- IME/USP, -- Prof. Daniel Macêdo Batista
 * Compilador: gcc linux 4.6.3
 * Editor: Sublime Text 2;
 * Sistema Operacional: Linux
 ******************************************************************************/

typedef struct superLong{
	unsigned long int *data;
	unsigned long int n;
} SuperLong;

void *mallocX (unsigned int nbytes);
void *reallocX (void *ptr, unsigned int nbytes);
void SuperLongSomaInt(SuperLong SL, unsigned long int k);
void startSuperLong(SuperLong *SL, long int p);
void readSuperLong(SuperLong *SL, char *str);
void freeSuperLong(SuperLong *SL);
void printSuperLong(SuperLong SL);