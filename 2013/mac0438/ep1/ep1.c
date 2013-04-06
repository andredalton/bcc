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
PosicaoAtleta **tempoEspaco;

int tic = 0;					/* O tic de tempo para impressão da classificação. ( default 30min, com -debug 1min ). */
int lPrint = 0;
int run;

/* Termino da declaração das variáveis globais. */

/*
int carregaArquivo(char nome[], int &hp, &mp, &ha, &ma, ){
	FILE *ent = fopen(nome, "r");
	
	
}
*/

/* Imprime a possição dos atletas a cada t segundos de prova. */
void *classificacao(int n) {
	int i=5;
	

	while(run){
		for(i=0; i<n; i++)
			if(tempoEspaco[tic][i].id==0) break;
		
		printf("(%d %d) \n", tic, i);

		if(i==n){
			printf("Imprimindo linha: %d\n", tic);
			tic++;
		}
		sleep(1);
	}

	printf("Ahhhhhh\n");
	
	/*
	relatividade = reallocX(NULL, sizeof(PosicaoAtleta)*m);
	relatividade = reallocX(relatividade, sizeof(PosicaoAtleta)*m++);
	relatividade = reallocX(relatividade, sizeof(PosicaoAtleta)*m++);
	relatividade = reallocX(relatividade, sizeof(PosicaoAtleta)*m++);
	relatividade = reallocX(relatividade, sizeof(PosicaoAtleta)*m++);
	relatividade = reallocX(relatividade[1], sizeof(PosicaoAtleta)*m--);
	relatividade = reallocX(relatividade[1], sizeof(PosicaoAtleta)*m--);
	*/

	return NULL;
}

void *atleta(Atleta a){
	int i;
	double p = 0;

	for(i=0; i<10; i++){
		/*
		printf("AA\t%d %f\n", a->id, p);
		atualizaPosicao( &tempoEspaco[i][a->id], a->id, p );
		*/
		tempoEspaco[i][a->id].id = a->id;
		tempoEspaco[i][a->id].posicao = p;
		printf("\t\t[%d, %d] = %f\n", i, a->id, p);
		p += (double) (rand()%500);
		sleep(rand()%5);
	}

	return NULL;
}

/* Função principal de verdade */
int ironMain(int argc, char *argv[]){
	int
		i,
		j,
		n=10;
	
	pthread_t ids_atletas[10];
	pthread_t id_classificacao;
	
	srand( time(NULL) );

	tempoEspaco = (PosicaoAtleta**) mallocX(10*sizeof(PosicaoAtleta*));
	
	printf("A\n");

	for(i=0; i<10; i++){
		printf("B\n");
		tempoEspaco[i] = novasPossicoes(n);
		for (j = 0; j < n; ++j)
		{
			tempoEspaco[i][j].id = 0;
			tempoEspaco[i][j].posicao = 0;
		}
	}

	run = 1;

	printf("Inicio das threads.\n");

	pthread_create(&(id_classificacao),NULL,  classificacao, (void *)n);

	for (i = 0; i < 10; i++) {
		if (pthread_create(&(ids_atletas[i]),NULL, atleta, (void *) novoAtleta(FEMININO, PROFISSIONAL, i) )) {
			fprintf(stderr,"Erro no pthread_create\n");
			return(2);
		}
	}
	
	for(i=0;i < 10;i++)
		pthread_join(ids_atletas[i],NULL); /* Esperara a junção das threads */

	run = 0;

	pthread_join(id_classificacao,NULL);

	printf("Termino de todas as threads.\n");
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

/* Função principal. */
int main(int argc, char *argv[]){
	ironMain(argc, argv);

	return 0;
}