#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h> /* Para trabalhar com threads */
#include <string.h>
#include <unistd.h>

#define NPROVAS 3
#define NATACAO 0
#define CICLISMO 1
#define CORRIDA 2

#define MASCULINO 0
#define FEMININO 1

#define PROFISSIONAL 0
#define AMADOR 1

#define SUBIDA 0
#define PLANO 1
#define DESCIDA 2

#define NATAM 38         /* x100m */ 
#define CITAM 180        /* x1km  */
#define COTAM 42         /* x1km  */

typedef struct tmp{
    unsigned int h;
	unsigned int m;
	unsigned int ms;
} *Tempo;

typedef struct atl{
	int sexo;
	int categoria;
	int ms[NPROVAS];
} *Atleta;

typedef struct posAtleta{
	int id;
	double position;
} PosicaoAtleta;

int PortalT1Ent;
int PortalT2Ent;
int PortalT1Sai;
int PortalT2Sai;
int *estrada[180];

int deltaTime=1800000;
int linEntregue=0;
int maxTime=0;
PosicaoAtleta **relatividade;

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

Atleta novoAtleta(int sexo, int categoria ) {
	int i;
	Atleta a = (Atleta) mallocX(sizeof(struct atl));
	
	a->sexo = sexo;
	a->categoria = categoria;
	for(i=0; i<NPROVAS; i++){
		a->ms[i] = 0;
	}
	return a;
}

/*
int carregaArquivo(char nome[], int &hp, &mp, &ha, &ma, ){
	FILE *ent = fopen(nome, "r");
	
	
}
*/

/* Função que calcula um tempo aleatório que tem valor minimo de min e máximo de max. */
int timeRand(int min, int max){
	return (int)(1000.0*min + 1000.0*rand()/RAND_MAX*(max-min));
}

void natacao(Atleta a){
	int i;
	
	for(i=0; i<NATAM; i++){
		if( a->sexo == FEMININO && a->categoria == AMADOR )
			a->ms[NATACAO] += timeRand(150, 300);
		if( a->sexo == MASCULINO && a->categoria == AMADOR )
			a->ms[NATACAO] += timeRand(120, 240);
		if( a->sexo == FEMININO && a->categoria == PROFISSIONAL )
			a->ms[NATACAO] += timeRand(90, 120);
		if( a->sexo == MASCULINO && a->categoria == PROFISSIONAL )
			a->ms[NATACAO] += timeRand(75, 100);
	}
}

void corrida(Atleta a){
	int i;
	
	for(i=0; i<COTAM; i++){
		if( a->sexo == FEMININO && a->categoria == AMADOR )
			a->ms[NATACAO] += timeRand(330, 420);
		if( a->sexo == MASCULINO && a->categoria == AMADOR )
			a->ms[NATACAO] += timeRand(300, 420);
		if( a->sexo == FEMININO && a->categoria == PROFISSIONAL )
			a->ms[NATACAO] += timeRand(260, 290);
		if( a->sexo == MASCULINO && a->categoria == PROFISSIONAL )
			a->ms[NATACAO] += timeRand(240, 260);
	}
}

int ciclismo( Atleta a, int terreno){
	switch(terreno){
		case SUBIDA:
			if( a->sexo == FEMININO && a->categoria == AMADOR )
				return timeRand(3600/10, 3600/15);
			if( a->sexo == MASCULINO && a->categoria == AMADOR )
				return timeRand(3600/12, 3600/20);
			if( a->sexo == FEMININO && a->categoria == PROFISSIONAL )
				return timeRand(3600/20, 3600/30);
			if( a->sexo == MASCULINO && a->categoria == PROFISSIONAL )
				return timeRand(3600/30, 3600/40);
			break;
		case PLANO:
			if( a->sexo == FEMININO && a->categoria == AMADOR )
				return timeRand(3600/25, 3600/40);
			if( a->sexo == MASCULINO && a->categoria == AMADOR )
				return timeRand(3600/30, 3600/45);
			if( a->sexo == FEMININO && a->categoria == PROFISSIONAL )
				return timeRand(3600/45, 3600/50);
			if( a->sexo == MASCULINO && a->categoria == PROFISSIONAL )
				return timeRand(3600/50, 3600/55);
			break;
		case DESCIDA:
			if( a->sexo == FEMININO && a->categoria == AMADOR )
				return timeRand(3600/50, 3600/65);
			if( a->sexo == MASCULINO && a->categoria == AMADOR )
				return timeRand(3600/50, 3600/70);
			if( a->sexo == FEMININO && a->categoria == PROFISSIONAL )
				return timeRand(3600/70, 3600/90);
			if( a->sexo == MASCULINO && a->categoria == PROFISSIONAL )
				return timeRand(3600/100, 3600/100);
			break;
	}
	return 0;
}

void classificacao(void){
	int i;
	
	for( i=0; i<5; i++){
		printf("%d\n", i);
		sleep(rand()%5);
	}
	
	/*
	relatividade = reallocX(NULL, sizeof(PosicaoAtleta)*m);
	relatividade = reallocX(relatividade, sizeof(PosicaoAtleta)*m++);
	relatividade = reallocX(relatividade, sizeof(PosicaoAtleta)*m++);
	relatividade = reallocX(relatividade, sizeof(PosicaoAtleta)*m++);
	relatividade = reallocX(relatividade, sizeof(PosicaoAtleta)*m++);
	relatividade = reallocX(relatividade[1], sizeof(PosicaoAtleta)*m--);
	relatividade = reallocX(relatividade[1], sizeof(PosicaoAtleta)*m--);
	*/
}

int main(int argc, char *argv[]){
	char *filename=NULL;
	int i;
	int m = 100;
	
	pthread_t *atleta;
	pthread_t ids_classificacao[10];
	
	srand( time(NULL) );
	
	for (i = 0; i < 10; i++) {
		if (pthread_create(&(ids_classificacao[i]),NULL,(void *)classificacao,NULL)) {
			fprintf(stderr,"Erro no pthread_create\n");
			return(2);
		}
	}
	
	/*
	for( i=1; i<argc; i++){
		if( strcmp(argv[i], "-debug")==0 )
			deltaTime = 60000;
		else
			filename = argv[i];
	}
	
	if(filename==NULL)
		return 1;
	*/
	return 0;
}
