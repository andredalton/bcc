#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h> /* Para trabalhar com threads */
#include <string.h>
#include <unistd.h>

#include "tempos.h"

/* Inicialização das variáveis globais. */
int PortalT1Ent;
int PortalT2Ent;
int PortalT1Sai;
int PortalT2Sai;
int *estrada[3][180];
PosicaoAtleta **tempoEspaco = NULL;

int tic;		/* O tic de tempo para impressão da classificação. ( default 30min, com -debug 1min ). */
int lPrint;
int run;
int natletas;
int deltaTime;

/* Termino da declaração das variáveis globais. */

void imprimeClassificacao(){
	printf("Imprimindo linha: %d\n", tic);
}

/* Imprime a possição dos atletas a cada t segundos de prova. */
void *classificacao(int n) {
	int i;
	
	while(run){
		if( (tic-lPrint)>0 ){
			for(i=0; i<n; i++)
				if(tempoEspaco[tic][i].id==-1) break;
			/* Inicio da sessao critica. */
			if(i==n){
				imprimeClassificacao();
				lPrint++;

				tempoEspaco = reallocX(tempoEspaco[1], sizeof(PosicaoAtleta*)*(tic-lPrint));
			}
			/* Termino da sessao critica. */
		}
		sleep(1);
	}

	printf("Classificacao final:\n");
	
	return NULL;
}

void *atleta(Atleta a){
	int i;
	int t;
	double p;
	/* Natação */
	printf("B\n");
	for(i=0; i<NATAM; i++){
		printf("C\n");
		natacao(a);
		printf("D\n");
		if( a->ms[NATACAO]/deltaTime != t/deltaTime ){
			/* Inicio da sessao critica. */
			if( a->ms[NATACAO]/deltaTime > tic ){
				tic++;
				printf("F\n");
				tempoEspaco = reallocX(tempoEspaco, sizeof(PosicaoAtleta*)*(tic+1-lPrint) );
				tempoEspaco[tic-lPrint] = novasPossicoes(natletas);
			}
			/* Aqui acaba a sessao critica. */

			printf("E\n");
			p = 100*(i-1) + 100*( (a->ms[NATACAO]-a->ms[NATACAO]%deltaTime)-t)/(a->ms[NATACAO]-t);
			atualizaPosicao( &tempoEspaco[i][a->id], a->id, p );
		}
	}

	/* T1 */

	/* Ciclismo */

	/* T2 */

	/* Corrida */

	return NULL;
}

/* Função principal de verdade */
int ironMain(int argc, char *argv[]){
	char *filename;
	int
		i,
		j,
		n=10;
	
	pthread_t *ids_atletas;
	pthread_t id_classificacao;
	
	srand( time(NULL) );

	/* Inicializando variáveis globais. */
	tempoEspaco = NULL;	
	tic = 0;
	lPrint = 0;
	natletas = 10;
	deltaTime = 1800000;
	run = 1;

	for( i=1; i<argc; i++){
		if( strcmp(argv[i], "-debug")==0 )
			deltaTime = 60000;
		else
			filename = argv[i];
	}

	/*	
	if(filename==NULL)
		return 1;
	*/

	printf("Inicio das threads.\n");

	pthread_create(&(id_classificacao),NULL,  classificacao, (void *)n);

	ids_atletas = mallocX(natletas*sizeof(pthread_t));
	for (i=0; i<natletas; i++) {
		printf("A\n");
		if (pthread_create(&(ids_atletas[i]),NULL, atleta, (void *) novoAtleta(FEMININO, PROFISSIONAL, i) )) {
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

/* Função principal. */
int main(int argc, char *argv[]){
	ironMain(argc, argv);

	return 0;
}