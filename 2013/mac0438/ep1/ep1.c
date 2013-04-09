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


/* Guarda a posição de cada um dos atletas como uma trinca entre o id, tempo e espaço.
*  Sendo que a primeira linha serve para guardar o tempo total de percurso e as demais
*  linhas o tempo pra cada um dos tics que deve ser mostrado.
*/
PosicaoAtleta **tempoEspaco = NULL;

int ticMax;
int run;
int natletas;
int deltaTime;
int debug;

/* Termino da declaração das variáveis globais. */

/* Imprime a classificação dos atletas. */
void imprimeClassificacao(int lPrint){
	if(1){
		if(debug)
			printf("Classificacao %dmin:\n", lPrint+1);
		else
			printf("Classificacao %dmin:\n", 30*(lPrint+1));

		printf("\t%d %f\n", tempoEspaco[lPrint][0].id, tempoEspaco[lPrint][0].posicao );
	}
}

/* Função usada na thread que imprime a classificação dos atletas a cada tic de tempo. */
void *classificacao(void) {
	int i;
	int lPrint=1;			/* Numero do tic impresso (1 ou 30min). */

	printf("Inicio classificacao\n");

	while(run){
		printf("\r");
		for(i=0; i<natletas; i++)
			if(tempoEspaco[lPrint][i].id==-1) break;		
		if(i==natletas){
			imprimeClassificacao(lPrint);
			lPrint++;
		}
	}

	while(lPrint<ticMax){
		imprimeClassificacao(lPrint);
		lPrint++;
	}

	printf("Classificacao final:\n");
	printf("\t%d %d %f\n", tempoEspaco[0][0].id, tempoEspaco[0][0].ms, tempoEspaco[0][0].posicao );

	return NULL;
}

/* Função usada na thread atleta. */
void *atleta(Atleta a){
	int i;
	int t=0;
	double p;
	/* Natação */
	for(i=0; i<NATAM; i++){
		natacao(a);
		if( tempoTotal(a)/deltaTime != t/deltaTime ){
			p = 100*i + 100*( (tempoTotal(a)-tempoTotal(a)%deltaTime)-t)/(tempoTotal(a)-t);
			atualizaPosicao( &tempoEspaco[tempoTotal(a)/deltaTime][a->id], a->id, (tempoTotal(a)-tempoTotal(a)%deltaTime), p );
			t = tempoTotal(a);
		}
	}
	
	/* T1 */
	/* Sessao critica!!! */
	t = tempoTotal(a);
	transicao(a, T1);
	if( tempoTotal(a)/deltaTime != t/deltaTime ){
		atualizaPosicao( &tempoEspaco[tempoTotal(a)/deltaTime][a->id], a->id, (tempoTotal(a)-tempoTotal(a)%deltaTime), 100*NATAM );
	}
	t = tempoTotal(a);
	/* Sessao critica!!! */

	/* Sessao critica!!! *
	/* Ciclismo */
	for(i=0; i<CITAM; i++){
		ciclismo(a, PLANO);
		if( tempoTotal(a)/deltaTime != t/deltaTime ){
			p = 1000*i + 1000*( (tempoTotal(a)-tempoTotal(a)%deltaTime)-t)/(tempoTotal(a)-t);
			atualizaPosicao( &tempoEspaco[tempoTotal(a)/deltaTime][a->id], a->id, (tempoTotal(a)-tempoTotal(a)%deltaTime), p );
			t = tempoTotal(a);
		}
	}	
	/* Sessao critica!!! *

	/* Sessao critica!!! */
	/* T2 */
	t = tempoTotal(a);
	transicao(a, T2);
	if( tempoTotal(a)/deltaTime != t/deltaTime ){
		atualizaPosicao( &tempoEspaco[tempoTotal(a)/deltaTime][a->id], a->id, (tempoTotal(a)-tempoTotal(a)%deltaTime), 100*NATAM + 1000*CITAM);
	}
	t = tempoTotal(a);
	/* Sessao critica!!! */

	/* Corrida */
	for(i=0; i<COTAM; i++){
		corrida(a);
		if( tempoTotal(a)/deltaTime != t/deltaTime ){
			p = 100*NATAM + 1000*i + 1000*( (tempoTotal(a)-tempoTotal(a)%deltaTime)-t)/(tempoTotal(a)-t);
			atualizaPosicao( &tempoEspaco[tempoTotal(a)/deltaTime][a->id], a->id, (tempoTotal(a)-tempoTotal(a)%deltaTime), p );
			t = tempoTotal(a);
		}
	}

	/* Aqui deve colocar o tempo final */

	atualizaPosicao( &tempoEspaco[0][a->id], a->id, tempoTotal(a), 100*NATAM + 1000*CITAM + 1000*COTAM );

	if(ticMax<tempoTotal(a)/deltaTime)
		ticMax = tempoTotal(a)/deltaTime;

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
	natletas = 1;
	deltaTime = 1800000;
	run = 1;
	ticMax = 0;

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