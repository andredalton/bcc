#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h> /* Para trabalhar com threads */
#include <string.h>
#include <unistd.h>

#include "defines.h"

void *mallocX (unsigned int nbytes) 
{
   void *ptr;
   ptr = malloc (nbytes);
   if (ptr == NULL) {
      printf ("Socorro! malloc devolveu NULL! (%d)\n", nbytes);
      exit (EXIT_FAILURE);
   }
   return ptr;
}

void *reallocX (void *ptr, unsigned int nbytes) 
{
   ptr = realloc( ptr, nbytes);
   if (ptr == NULL) {
      printf ("Socorro! realloc devolveu NULL! (%d)\n", nbytes);
      exit (EXIT_FAILURE);
   }
   return ptr;
}

Tempo converteTempo(int ms){
	Tempo t = (Tempo) mallocX(sizeof(struct tmp));
	
	t->ms = ms%60000;
	t->m = ms%3600000/60000;
	t->h = ms/3600000;
	
	return t;
}

Atleta novoAtleta(int sexo, int categoria, char *nome, char *sobrenome, int id) {
	int i;
	Atleta a = (Atleta) mallocX(sizeof(struct atl));
	
	a->id = id;
	a->nome = nome;
	a->sobrenome = sobrenome;
	a->sexo = sexo;
	a->categoria = categoria;
	for(i=0; i<ETAPAS; i++){
		a->ms[i] = 0;
	}
	return a;
}

int tempoTotal (Atleta a){
	int i, T=0;

	for ( i=0; i<ETAPAS; i++)	{
		T += a->ms[i];
	}

	return T;
}

PosicaoAtleta *novasPossicoes(int n){
	int i;
	PosicaoAtleta *p = (PosicaoAtleta *) mallocX(n*sizeof(struct posAtleta));

	for ( i=0; i < n; i++)
	{
		p[i].id = -1;
		p[i].ms = 0;
		p[i].posicao = 0;
	}

	return p;
}

void atualizaPosicao(PosicaoAtleta *p, int id, int t, double posicao){
	p->id = id;
	p->ms = t;
	p->posicao = posicao;
}

ListName listaNomes(char entrada[]){
	FILE *fe = fopen(entrada, "r");
	char buffer[100];
	char nome[80];
	ListName lista = (ListName) mallocX(sizeof(struct listName));
	int i, k;

	lista->max = 0;

	for( i=0; !feof(fe) && i<MAXNAMES ; i++){
		fgets( buffer, 100, fe);
		sscanf(buffer, "%s %d", lista->nome[i], &k);
		lista->max += k;
		lista->prob[i] = lista->max;
	}
	fclose(fe);

	return lista;
}

char *randomName(ListName L){
	int
		i,
		k=rand()%L->max;

	for( i=0; i<MAXNAMES; i++)
		if(k<L->prob[i]) break;

	return L->nome[i];
}