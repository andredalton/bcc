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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#include "precisao.h"

void *mallocX (unsigned int nbytes) {
   void *ptr;
   ptr = malloc (nbytes);
   if (ptr == NULL) {
      printf ("Socorro! malloc devolveu NULL! (%d)\n", nbytes);
      exit (EXIT_FAILURE);
   }
   return ptr;
}

void *reallocX (void *ptr, unsigned int nbytes) {
   ptr = realloc( ptr, nbytes);
   if (ptr == NULL) {
      printf ("Socorro! realloc devolveu NULL! (%d)\n", nbytes);
      exit (EXIT_FAILURE);
   }
   return ptr;
}

void startSuperLong(SuperLong *SL, long int p) {
	SL->n = p;
	if(p>0)
      SL->data = (unsigned long int*) mallocX(p*sizeof(unsigned long int));
   else
      SL->data = NULL;
}

void freeSuperLong(SuperLong *SL) {
	free(SL->data);
}

void readSuperLong(SuperLong *SL, char *str){
   int tam;
   char *buffer;
   long int aux;

   startSuperLong(SL, 0);

   /* Lendo um decimal com o número escrito em letras */
   tam = (int)log10(LONG_MAX);
   buffer = (char*)mallocX( tam+1 );
   for(SL->n=0; SL->n*tam<strlen(str); SL->n++) {
      SL->data = (unsigned long int*) reallocX( SL->data, (SL->n+1)*sizeof(unsigned long int) );
      memcpy ( buffer, &str[SL->n*tam], (tam)*sizeof(char) );
      buffer[tam] = '\0';
      sscanf(buffer, "%ld", &aux);
      SL->data[SL->n] = aux;
   }
}

void SuperLongSomaInt(SuperLong SL, unsigned long int k){
   int
      i,
      mem = k,
      tam = (int)log10(LONG_MAX);

   for( i = SL.n-1; i>=0 && mem!=0; i--){
      SL.data[i] += mem;
      mem = (int) SL.data[i]/pow(10, tam);
   }
}

void printSuperLong(SuperLong SL) {
	int i, j;
   char prt[10];

   sprintf(prt, "%%0%dld", (int)log10(LONG_MAX) );

   printf("Super long: ");
   printf("(dec)\n");
   for(i=0; i<SL.n; i++)
      printf(prt, SL.data[i]);
}