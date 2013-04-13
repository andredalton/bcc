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
	for ( i=0; i<ETAPAS; i++)
		T += a->ms[i];
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

/* Recebe dois pondeiros para a estrutura 'PosicaoAtleta' e compara-os (ordem).
 * Retorna int > 0 se p1 '>' p2, int == 0 se p1 '==' p2 e int < 0 c.c.
 ******************************************************************************/
int comparePosicaoAtleta( const void *p1, const void *p2) {
   int r;
   PosicaoAtleta *a, *b;
   a = (PosicaoAtleta *) p1;
   b = (PosicaoAtleta *) p2;
   r = ((int) 100*b->posicao - (int) 100*a->posicao);
   if(r) return r;
   return ( a->ms - b->ms );
}

void ordenaPosicaoAtleta( PosicaoAtleta *vet, int tam_vet) {
   qsort( vet, tam_vet, sizeof (PosicaoAtleta), (void *) comparePosicaoAtleta);
}

ListName listaNomes(char entrada[]){
	FILE *fe = fopen(entrada, "r");
	char buffer[100];
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

int punicaoR(int *v, int ini, int fim, int tam, int tmp, int faixas){
	int
		uso,
		i,
		m = (ini+fim)/2;

	if(ini==fim){
		if (v[ini]<tmp) {
			memcpy ( v, v+1, ini*sizeof(int) );
			v[ini] = tmp;
		}
		else if(v[ini]>tmp) {
			memcpy ( v, v+1, (ini)*sizeof(int) );
			v[ini-1] = tmp;
		}
		else if(v[ini]==tmp) {
			uso = 1;  /* uso = 1 pois já tem alguem 'usando' uma via */
			if (uso+1>faixas) { /* uso + 1 pois estou tentando 'usar' uma via */
				return 3 + punicaoR(v, fim, tam-1, tam, tmp+3, faixas);
			}
			memcpy ( v, v+1, ini*sizeof(int) );
			v[ini] = tmp;
			return 3;
		}
		return 0;
	}
	else if (v[m]==tmp){
		uso=1; /* uso = 1 pois já tem alguem 'usando' uma via */

		for( i=1; i<faixas && faixas+i<tam; i++, uso++)
			if(v[m+i]!=tmp) break;
		for( i=1; i<faixas && faixas-i>0; i++, uso++)
			if(v[m-i]!=tmp) break;

		/* Caso o atleta esteja tentando ultrapassar alguém sem ter uma via, será punido!!! */
		if(uso+1>faixas) /* uso + 1 pois estou tentando 'usar' uma via */
			return 3 + punicaoR(v, m, fim, tam, tmp+3, faixas);
		else{
			memcpy ( v, v+1, m*sizeof(int) );
			v[m] = tmp;
			return 0;
		}
	}
	else if ( v[m]<tmp )
		return punicaoR(v, m+1, fim, tam, tmp, faixas);
	else
		return punicaoR(v, ini, m-1, tam, tmp, faixas);
}

int punicao(int *v, int tam, int tmp, int faixas){
	return punicaoR(v, 0, tam-1, tam, tmp, faixas);
}
