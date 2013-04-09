#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h> /* Para trabalhar com threads */
#include <string.h>
#include <unistd.h>

#include "tempos.h"

#define ironMain main

/* Inicialização das variáveis globais. */
int PortalT1Ent;
int PortalT2Ent;
int PortalT1Sai;
int PortalT2Sai;
int *estrada[3][180];
PosicaoAtleta **tempoEspaco = NULL;

int lPrint;			/* Numero do tic impresso (1 ou 30min). */
int run;
int natletas;
int deltaTime;
int debug;

/* Termino da declaração das variáveis globais. */

/* Imprime a classificação dos atletas. */
void imprimeClassificacao(){
	if(debug)
		printf("Classificacao %dmin:\n", lPrint);
	else
		printf("Classificacao %dmin:\n", 30*lPrint);

	printf("\t%d %f\n", tempoEspaco[lPrint][0].id, tempoEspaco[lPrint][0].posicao );
}

/* Função usada na thread que imprime a classificação dos atletas a cada tic de tempo. */
void *classificacao(void) {
	int i;
	
	while(run){
		for(i=0; i<natletas; i++)
			if(tempoEspaco[lPrint][i].id==-1) break;
		if(i==natletas){
			imprimeClassificacao();
			lPrint++;
		}
	}

	printf("Classificacao final:\n");
	printf("\t%d %f\n", tempoEspaco[lPrint][0].id, tempoEspaco[lPrint][0].posicao );
		
	return NULL;
}

/* Função usada na thread atleta. */
void *atleta(Atleta a){
	int i;
	int t;
	double p;
	/* Natação */
	for(i=0; i<NATAM; i++){
		natacao(a);
		/*
		printf("\t[%d] %d - %d\n", i, a->ms[NATACAO], a->ms[NATACAO]/deltaTime );
		*/
		if( a->ms[NATACAO]/deltaTime != t/deltaTime ){
			p = 100*i + 100*( (a->ms[NATACAO]-a->ms[NATACAO]%deltaTime)-t)/(a->ms[NATACAO]-t);
			/*
			printf("\t%f\n", p);
			*/
			atualizaPosicao( &tempoEspaco[i][a->id], a->id, p );
		}
	}

	/* T1 */

	/* Ciclismo */

	/* T2 */

	/* Corrida */

	return NULL;
}

/* Função principal. */
int ironMain(int argc, char *argv[]){
	char *filename;
	int
		i,
		j,
		debug=0;
	
	pthread_t *ids_atletas;
	pthread_t id_classificacao;
	
	srand( time(NULL) );

	/* Inicializando variáveis globais. */
	tempoEspaco = NULL;	
	lPrint = 0;
	natletas = 1;
	deltaTime = 1800000;
	run = 1;

	for( i=1; i<argc; i++){
		if( strcmp(argv[i], "-debug")==0 ){
			deltaTime = 60000;
			debug=1;
		}
		else
			filename = argv[i];
	}

	/* Alocando a quantidade máxima de tics necessária para guardar as informações de tempo dos atletas. */
	tempoEspaco = (PosicaoAtleta **) mallocX( (29*debug+1)*TMAX*sizeof(PosicaoAtleta*) );
	for( i=0; i<(59*debug+1)*TMAX; i++)
		tempoEspaco[i] = novasPossicoes(natletas);

	/*
	if(filename==NULL)
		return 1;
	*/

	printf("Inicio das threads.\n");

	pthread_create(&(id_classificacao),NULL,  classificacao, NULL);

	ids_atletas = mallocX(natletas*sizeof(pthread_t));
	for (i=0; i<natletas; i++) {
		printf("Criando thread #%d\n", i);
		if (pthread_create(&(ids_atletas[i]),NULL, atleta, (void *) novoAtleta(MASCULINO, PROFISSIONAL, i) )) {
			fprintf(stderr,"Erro no pthread_create\n");
			return(2);
		}
	}
	
	for(i=0; i<natletas; i++)
		pthread_join(ids_atletas[i],NULL); /* Esperara a junção das threads */

	run = 0;

	pthread_join(id_classificacao,NULL);

	printf("Termino de todas as threads.\n");
	
	return 0;
}