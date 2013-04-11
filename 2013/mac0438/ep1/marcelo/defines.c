/***********
 * defines.c
 ***********/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h> /* Para trabalhar com threads */
#include <string.h>
#include <unistd.h>

#include "defines.h"

void *mallocX( unsigned int nbytes) {
   void *ptr;
   ptr = malloc( nbytes);
   if (ptr == NULL) {
      printf( "Socorro! malloc devolveu NULL! (%d)\n", nbytes);
      exit (EXIT_FAILURE);
   }
   return ptr;
}

void *reallocX( void *ptr, unsigned int nbytes) {
   ptr = realloc( ptr, nbytes);
   if (ptr == NULL) {
      printf( "Socorro! realloc devolveu NULL! (%d)\n", nbytes);
      exit (EXIT_FAILURE);
   }
   return ptr;
}

Tempo converteTempo( int ms) {
   Tempo t = (Tempo) mallocX( sizeof (struct tmp));
   
   t->ms = ms % 60000;
   t->m = ms % 3600000 / 60000;
   t->h = ms / 3600000;
   
   return t;
}

Atleta novoAtleta( int sexo, int categoria, int id) {
   int i;
   Atleta a = (Atleta) mallocX( sizeof (struct atl));
   a->id = id;
   a->sexo = sexo;
   a->categoria = categoria;
   for (i = 0; i < ETAPAS; i++) {
      a->ms[i] = 0;
   }
   return a;
}

int tempoTotal( Atleta a) {
   int i, T=0;
   for (i = 0; i < ETAPAS; i++) {
      T += a->ms[i];
   }
   return T;
}

PosicaoAtleta *novasPossicoes( int n) {
   int i;
   PosicaoAtleta *p = (PosicaoAtleta *) mallocX( n * sizeof (struct posAtleta));
   for (i = 0; i < n; i++) {
      p[i].id = -1;
      p[i].ms = 0;
      p[i].posicao = 0;
   }
   return p;
}

void atualizaPosicao( PosicaoAtleta *p, int id, int t, double posicao) {
   p->id = id;
   p->ms = t;
   p->posicao = posicao;
}

/* Recebe dois pondeiros para a estrutura 'PosicaoAtleta' e compara-os (ordem).
 * Retorna int > 0 se p1 '>' p2, int == 0 se p1 '==' p2 e int < 0 c.c.
 ******************************************************************************/
int comparePosicaoAtleta( const void *p1, const void *p2) {
   PosicaoAtleta *a, *b;
   a = (PosicaoAtleta *) p1;
   b = (PosicaoAtleta *) p2;
   return ((int) a->posicao - (int) b->posicao);
}

/* Função modelo para comparar elementos de vetores (fornecida para a qsort()).
 ******************************************************************************/
int compare( const void *a, const void *b) {
   return ( *(int*)a - *(int*)b );
}