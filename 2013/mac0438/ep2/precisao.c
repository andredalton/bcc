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
      SL->data.l = (unsigned long int*) mallocX(p*sizeof(unsigned long int));
   else
      SL->data.l = NULL;
}

void freeSuperLong(SuperLong *SL) {
	free(SL->data.l);
}

void readSuperLong(SuperLong *SL, char *str, char b){
   int tam = (int)log10(ULONG_MAX)*sizeof(char);
   char *buffer = (char*)mallocX( tam+1 );
   long int aux;

   startSuperLong(SL, 0);

   if(b=='b' || b==2){
      SL->bin = 1;
   }
   else{
      SL->bin = 0;
      for(SL->n=0; SL->n*tam<strlen(str); SL->n++) {
         SL->data.l = (unsigned long int*) reallocX( SL->data.l, (SL->n+1)*sizeof(unsigned long int) );
         memcpy ( buffer, &str[SL->n*tam], (tam)*sizeof(char) );
         buffer[tam] = '\0';
         sscanf(buffer, "%ld", &aux);
         SL->data.l[SL->n] = aux;
      }
   }
}

void printSuperLong(SuperLong SL) {
	int i, j;

   printf("Super long: ");
   if(SL.bin){
      printf("(bin)\n");
      for(i=0; i<SL.n; i++)
         for(j=0; j<sizeof(long int); j++)
            printf("#%d ", SL.data.c[i*sizeof(long int)+j]);
   }
   else{
      printf("(dec)\n");
      for(i=0; i<SL.n; i++)
         printf("%ld", SL.data.l[i]);
   }
}