#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h> /* Para trabalhar com threads */
#include <string.h>
#include <unistd.h>

#include "tempos.h"

#define ironMain main

/* Inicialização das variáveis globais.
 **************************************/
int PortalT1Ent;
int PortalT2Ent;
int PortalT1Sai;
int PortalT2Sai;

int *TPortalT1Ent;
int *TPortalT2Ent;
int *TPortalT1Sai;
int *TPortalT2Sai;

/* Guarda a posição de cada um dos atletas como uma trinca entre o id, tempo e
 * espaço. Sendo que a primeira linha serve para guardar o tempo total de 
 * percurso e as demais linhas o tempo pra cada um dos tics que deve ser 
 * mostrado.
 *****************************************************************************/
PosicaoAtleta **tempoEspaco = NULL;
PosicaoAtleta **estrada = NULL;

int ticMax;
int run;
int natletas;
int deltaTime;
int debug;

Atleta *atletas;

/**********************************************
 * Termino da declaração das variáveis globais.
 **********************************************/

/* Imprime a classificação dos atletas.
 **************************************/
void imprimeClassificacao(int lPrint){
	int i;
	if(1){
		if(debug)
			printf("\nClassificacao %dmin:\n", lPrint);
		else
			printf("\nClassificacao %dmin:\n", 30*(lPrint));
	}
	for( i=0; i<3; i++){
		if(debug && i>=3) break;
		printf(" %d) \"%s %s\"\t%.2fm\n", (i+1), atletas[tempoEspaco[lPrint][i].id]->nome, atletas[tempoEspaco[lPrint][i].id]->sobrenome, tempoEspaco[lPrint][i].posicao);
	}
}

/* Função usada na thread que imprime a classificação dos atletas a cada tic de 
 * tempo.
 ******************************************************************************/
void *classificacao(void) {
	int i;
	int lPrint=1;			/* Numero do tic impresso (1 ou 30min). */
	int vencedores = 0;

	while(run){
		printf("\r");
		for(i=0; i<natletas; i++)
			if( tempoEspaco[lPrint][i].id==-1) break;
		if(i==natletas){
			ordenaPosicaoAtleta( tempoEspaco[lPrint], natletas);
			imprimeClassificacao(lPrint);
			lPrint++;
		}
	}

	while(lPrint<ticMax){
		imprimeClassificacao(lPrint);
		lPrint++;
	}

	printf("\nClassificacao final: %d\n", vencedores);
	printf("\t%d %d %f\n", tempoEspaco[0][0].id, tempoEspaco[0][0].ms, tempoEspaco[0][0].posicao );

	printf("\n\n\n%s %s\n", atletas[30]->nome, atletas[30]->sobrenome);

	return NULL;
}

/* Função usada na thread atleta.
 ********************************/
void *atleta(Atleta a){
	int i;
	int t=0;
	double p;
	/* Natação */
	for(i=0; i<NATAM; i++){
		natacao(a);
		if( tempoTotal(a)/deltaTime != t/deltaTime ){
			p = 100.0*i + 100.0*( (tempoTotal(a)-tempoTotal(a)%deltaTime)-t)/(tempoTotal(a)-t);
			atualizaPosicao( &tempoEspaco[tempoTotal(a)/deltaTime][a->id], a->id, (tempoTotal(a)-tempoTotal(a)%deltaTime), p );
			t = tempoTotal(a);
		}
	}
	
	/* T1 */
    /*******************
    * Sessao critica!!!
    *******************/
	t = tempoTotal(a);
	transicao(a, T1);
	if( tempoTotal(a)/deltaTime != t/deltaTime ){
		atualizaPosicao( &tempoEspaco[tempoTotal(a)/deltaTime][a->id], a->id, (tempoTotal(a)-tempoTotal(a)%deltaTime), 100*NATAM );
	}
	t = tempoTotal(a);
	/**********************
    * Termino S. critica!!!
    **********************/
    /*******************
    * Sessao critica!!!
    *******************/
    /* Ciclismo */
	for(i=0; i<CITAM; i++){
		ciclismo(a, PLANO);
		if( tempoTotal(a)/deltaTime != t/deltaTime ){
			p = 100*NATAM + 1000.0*i + 1000.0*( (tempoTotal(a)-tempoTotal(a)%deltaTime)-t)/(tempoTotal(a)-t);
			atualizaPosicao( &tempoEspaco[tempoTotal(a)/deltaTime][a->id], a->id, (tempoTotal(a)-tempoTotal(a)%deltaTime), p );
			t = tempoTotal(a);
		}
	}	
	/**********************
    * Termino S. critica!!!
    **********************/
    /*******************
    * Sessao critica!!!
    *******************/
	/* T2 */
	t = tempoTotal(a);
	transicao(a, T2);
	if( tempoTotal(a)/deltaTime != t/deltaTime ){
		atualizaPosicao( &tempoEspaco[tempoTotal(a)/deltaTime][a->id], a->id, (tempoTotal(a)-tempoTotal(a)%deltaTime), 100*NATAM + 1000*CITAM);
	}
	t = tempoTotal(a);
	/**********************
    * Termino S. critica!!!
    **********************/
	/* Corrida */
	for(i=0; i<COTAM; i++){
		corrida(a);
		if( tempoTotal(a)/deltaTime != t/deltaTime ){
			p = 100*NATAM + 1000*CITAM + 1000.0*i + 1000.0*( (tempoTotal(a)-tempoTotal(a)%deltaTime)-t)/(tempoTotal(a)-t);
			atualizaPosicao( &tempoEspaco[tempoTotal(a)/deltaTime][a->id], a->id, (tempoTotal(a)-tempoTotal(a)%deltaTime), p );
			t = tempoTotal(a);
		}
	}
	/* Guardadndo tempo final na linha correspondente na matriz tempoEspaco. */
	atualizaPosicao( &tempoEspaco[0][a->id], a->id, tempoTotal(a), 100*NATAM + 1000*CITAM + 1000*COTAM );

	if(ticMax<tempoTotal(a)/deltaTime)
		ticMax = tempoTotal(a)/deltaTime;

	return NULL;
}


/* Função principal.
 *******************/
int ironMain(int argc, char *argv[]){
	char *filename = NULL;
	FILE *entrada;
	int
		i,
		j,
		hp,
		mp,
		ha,
		ma;

	ListName H = listaNomes("homens.txt");
	ListName M = listaNomes("mulheres.txt");
	ListName S = listaNomes("sobrenomes.txt");
	


	pthread_t *ids_atletas;
	pthread_t id_classificacao;
	
	srand( time(NULL) );

	/* Inicializando variáveis globais. */
	tempoEspaco = NULL;	
	natletas = 1;
	deltaTime = 1800000;
	run = 1;
	ticMax = 0;
	debug = 0;

	/* Verificando os parametros de entrada. */
	for( i=1; i<argc; i++){
		if( strcmp(argv[i], "-debug")==0 ){
			deltaTime = 60000;
			debug=1;
		}
		else
			filename = argv[i];
	}

	if(filename==NULL){
		printf(
			"Modo de uso:\n"
			"%s -debug <filename>\n"
			"-debug: para rodar em modo de depuracao.\n"
			"<filename>: nome do arquivo de entrada\n", argv[0]
		);
		return 0;
	}

	entrada = fopen(filename, "r");
	fscanf(entrada, "%d\n%d\n%d\n%d", &hp, &mp, &ha, &ma);
	fclose(entrada);

	natletas = hp + mp + ha + ma;

	/* Alocando a quantidade máxima de tics necessária para guardar as informações de tempo dos atletas. */
	tempoEspaco = (PosicaoAtleta **) mallocX( (29*debug+1)*TMAX*sizeof(PosicaoAtleta*) );
	for( i=0; i<(59*debug+1)*TMAX; i++)
		tempoEspaco[i] = novasPossicoes(natletas);

	TPortalT1Ent = (int*) mallocX( natletas*sizeof(int) );
	TPortalT2Ent = (int*) mallocX( natletas*sizeof(int) );
	TPortalT1Sai = (int*) mallocX( natletas*sizeof(int) );
	TPortalT2Sai = (int*) mallocX( natletas*sizeof(int) );

	ids_atletas = mallocX(natletas*sizeof(pthread_t));

	atletas = (Atleta*) mallocX(natletas*sizeof(Atleta));

	for (i=0; i<natletas; ++i){
		TPortalT1Ent[i] = -1;
		TPortalT2Ent[i] = -1;
		TPortalT1Sai[i] = -1;
		TPortalT2Sai[i] = -1;
	}

	pthread_create(&(id_classificacao),NULL,  classificacao, NULL);

	printf("Criando atletas: \n");
	printf("\tMasculino Profissional: %d\n", hp);
	for (i=0; i<hp; i++) {
		atletas[i] = novoAtleta(MASCULINO, PROFISSIONAL, randomName(H), randomName(S), i);
		if (pthread_create(&(ids_atletas[i]),NULL, atleta, (void *) atletas[i] )) {
			fprintf(stderr,"Erro no pthread_create\n");
			return(2);
		}
	}

	printf("\tFeminino Profissional: %d\n", mp);
	for (; i<hp+mp; i++) {
		atletas[i] = novoAtleta(FEMININO, PROFISSIONAL, randomName(H), randomName(S), i);
		if (pthread_create(&(ids_atletas[i]),NULL, atleta, (void *) atletas[i] )) {
			fprintf(stderr,"Erro no pthread_create\n");
			return(2);
		}
	}

	printf("\tMasculino Amador: %d\n", ha);
	for (; i<hp+mp+ha; i++) {
		atletas[i] = novoAtleta(MASCULINO, AMADOR, randomName(H), randomName(S), i);
		if (pthread_create(&(ids_atletas[i]),NULL, atleta, (void *) atletas[i] )) {
			fprintf(stderr,"Erro no pthread_create\n");
			return(2);
		}
	}

	printf("\tFeminino Amador: %d\n", ma);
	for (i=0; i<hp+mp+ha+ma; i++) {
		atletas[i] = novoAtleta(FEMININO, AMADOR, randomName(H), randomName(S), i);
		if (pthread_create(&(ids_atletas[i]),NULL, atleta, (void *) atletas[i] )) {
			fprintf(stderr,"Erro no pthread_create\n");
			return(2);
		}
	}
	
	for(i=0; i<natletas; i++)
		pthread_join(ids_atletas[i],NULL); /* Esperara a junção das threads */

	run = 0;
	
	pthread_join(id_classificacao,NULL);
	
	printf("Termino de todas as threads.\n\n\n%d\n", ticMax);
	
	return 0;
}