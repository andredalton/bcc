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
int natletas;
int deltaTime;
int debug;

Atleta *atletas;

/**********************************************
 * Termino da declaração das variáveis globais.
 **********************************************/

/* Imprime a classificação dos atletas.
 **************************************/
void imprimeClassificacao(PosicaoAtleta *lista, int tempo){
	int i, j, tam;
	Tempo t;
	char sex, cat;
	
	for( i=0; i<natletas; i++){
		if(!debug && i>=3) break;

		sex = atletas[lista[i].id]->sexo==MASCULINO ? 'M' : 'F';
		cat = atletas[lista[i].id]->categoria==PROFISSIONAL ? 'P' : 'A';

		if(tempo) {
			t = converteTempo(lista[i].ms);
			/*
			printf(" %d) \"%s %s\"\t%dh%dm%06.3fs\n%d\n", (i+1), atletas[lista[i].id]->nome, atletas[lista[i].id]->sobrenome, t->h, t->m, (double)t->ms/1000, strlen(atletas[lista[i].id]->sobrenome) );
			*/
			printf("%4d) \"%s %s\"", (i+1), atletas[lista[i].id]->nome, atletas[lista[i].id]->sobrenome);
			tam = strlen(atletas[lista[i].id]->nome)+strlen(atletas[lista[i].id]->sobrenome);
			for (j=0; j<22-tam; j++) {
				printf(" ");
			}
			printf("%c   %c   %02dh%02dm%06.3fs\n", sex, cat, t->h, t->m, (double)t->ms/1000);
			free(t);
		}
		else {
			/*
			printf(" %d) \"%s %s\"\t%.2fm\n", (i+1), atletas[lista[i].id]->nome, atletas[lista[i].id]->sobrenome, lista[i].posicao);
			*/
			printf("%4d) \"%s %s\"", (i+1), atletas[lista[i].id]->nome, atletas[lista[i].id]->sobrenome);
			tam = log10(lista[i].posicao) + strlen(atletas[lista[i].id]->nome)+strlen(atletas[lista[i].id]->sobrenome);
			for (j=0; j<30-tam; j++) {
				printf(" ");
			}
			printf("%c   %c   ", sex, cat);
			printf("%.2fm\n", lista[i].posicao);
		}
	}
}

/* Função usada na thread que imprime a classificação dos atletas a cada tic de 
 * tempo.
 *
 * Obs: esta função imprime uma nova linha somente quando todos os atletas
 * finalizam o tic. Não para cada grupo.
 ******************************************************************************/
void *classificacao(void) {
	int
		i,
		mp,
		fp,
		ma,
		fa,
		lPrint=0,			/* Numero do tic impresso (1 ou 30min). */
		vmp = 0,
		vfp = 0,
		vma = 0,
		vfa = 0;

	PosicaoAtleta **grupos;

	if(!debug){
		grupos = (PosicaoAtleta **) mallocX(4*sizeof(PosicaoAtleta *));
		for(i=0; i<4; i++)
			grupos[i] = novasPossicoes(natletas);
	}

	while( lPrint<(29*debug+1)*TMAX && ! (vmp && vfp && vma && vfa) ){
		printf("\r");
		for(i=0; i<natletas; i++)
			if( tempoEspaco[lPrint][i].id==-1) break;
		if(i==natletas){
			ordenaPosicaoAtleta( tempoEspaco[lPrint], natletas);
			if(debug) {
				for(i=0; i<natletas; i++)
					if( tempoEspaco[lPrint][i].posicao != 100*NATAM + 1000*CITAM + 1000*COTAM) break;
				if( i==natletas ) vmp = vfp = vma = vfa = 1;
				printf("\nClassificacao %dmin:\n", lPrint+1);
				imprimeClassificacao(tempoEspaco[lPrint], 0);
			}
			else {
				/* Fragmentando os resultados já ordenados em grupos. */
				mp = fp = ma = fa = 0;
				for(i=0; i<natletas; i++){
					switch( atletas[tempoEspaco[lPrint][i].id]->sexo + 2*atletas[tempoEspaco[lPrint][i].id]->categoria ){
						case MASCULINO + 2 * PROFISSIONAL:
							if(!vmp) {
								atualizaPosicao( &grupos[MASCULINO + 2 * PROFISSIONAL][mp], tempoEspaco[lPrint][i].id, tempoEspaco[lPrint][i].ms, tempoEspaco[lPrint][i].posicao );
								mp++;
							}
							break;
						case FEMININO + 2 * PROFISSIONAL:
							if(!vfp) {
								atualizaPosicao( &grupos[FEMININO + 2 * PROFISSIONAL][fp], tempoEspaco[lPrint][i].id, tempoEspaco[lPrint][i].ms, tempoEspaco[lPrint][i].posicao );
								fp++;
							}
							break;
						case MASCULINO + 2 * AMADOR:
							if(!vma) {
								atualizaPosicao( &grupos[MASCULINO + 2 * AMADOR][ma], tempoEspaco[lPrint][i].id, tempoEspaco[lPrint][i].ms, tempoEspaco[lPrint][i].posicao );
								ma++;
							}
							break;
						case FEMININO + 2 * AMADOR:
							if(!vfa) {
								atualizaPosicao( &grupos[FEMININO + 2 * AMADOR][fa], tempoEspaco[lPrint][i].id, tempoEspaco[lPrint][i].ms, tempoEspaco[lPrint][i].posicao );
								fa++;
							}
							break;
					}
				}

				/* Verificando o término de cada um dos grupos em separado. */
				for(i=0; i<3 && !vmp; i++)
					if( grupos[MASCULINO + 2 * PROFISSIONAL][i].posicao != 100*NATAM + 1000*CITAM + 1000*COTAM) break;
				if(i==3 || i==mp)
					vmp=1;

				for(i=0; i<3 && !vfp; i++)
					if( grupos[FEMININO + 2 * PROFISSIONAL][i].posicao != 100*NATAM + 1000*CITAM + 1000*COTAM) break;
				if(i==3 || i==mp)
					vfp=1;

				for(i=0; i<3 && !vma; i++)
					if( grupos[MASCULINO + 2 * AMADOR][i].posicao != 100*NATAM + 1000*CITAM + 1000*COTAM) break;
				if(i==3 || i==mp)
					vma=1;

				for(i=0; i<3 && !vfa; i++)
					if( grupos[FEMININO + 2 * AMADOR][i].posicao != 100*NATAM + 1000*CITAM + 1000*COTAM) break;
				if(i==3 || i==mp)
					vfa=1;

				/* Imprimindo a classificação ou finalização quando necessário pra cada um dos grupos. */
				if(!vmp){
					printf("\nClassificacao %dmin:  (Masculino Profissional)\n", 30*(lPrint+1));
					imprimeClassificacao(grupos[MASCULINO + 2 * PROFISSIONAL], 0);
				}
				else if(vmp==1){
					printf("\nClassificacao Final:  (Masculino Profissional)\n");
					imprimeClassificacao(grupos[MASCULINO + 2 * PROFISSIONAL], 1);
					vmp = 2;
				}

				if(!vfp){
					printf("\nClassificacao %dmin:  (Feminino Profissional)\n", 30*(lPrint+1));
					imprimeClassificacao(grupos[FEMININO + 2 * PROFISSIONAL], 0);
				}
				else if(vfp==1){
					printf("\nClassificacao Final:  (Feminino Profissional)\n");
					imprimeClassificacao(grupos[FEMININO + 2 * PROFISSIONAL], 1);
					vfp = 2;
				}

				if(!vma){
					printf("\nClassificacao %dmin:  (Masculino Amador)\n", 30*(lPrint+1));
					imprimeClassificacao(grupos[MASCULINO + 2 * PROFISSIONAL], 0);
				}
				else if(vma==1){
					printf("\nClassificacao Final:  (Masculino Amador)\n");
					imprimeClassificacao(grupos[MASCULINO + 2 * AMADOR], 1);
					vma = 2;
				}

				if(!vfa){
					printf("\nClassificacao %dmin:  (Feminino Amador)\n", 30*(lPrint+1));
					imprimeClassificacao(grupos[FEMININO + 2 * AMADOR], 0);
				}
				else if(vfa==1){
					printf("\nClassificacao Final:  (Feminino Amador)\n");
					imprimeClassificacao(grupos[FEMININO + 2 * AMADOR], 1);
					vfa = 2;
				}
			}
			lPrint++;
		}
	}

	return NULL;
}

/* Função usada na thread atleta.
 ********************************/
void *atleta(Atleta a){
	int i, j;
	int t=0;
	double p;
	/* Natação */
	for(i=0; i<NATAM; i++){
		natacao(a);
		for( j=0; j<tempoTotal(a)/deltaTime-t/deltaTime; j++) {
			p = 100.0*i + 100.0*( ( j*deltaTime + tempoTotal(a)-tempoTotal(a)%deltaTime)-t)/(tempoTotal(a)-t);
			atualizaPosicao( &tempoEspaco[t/deltaTime+j][a->id], a->id, (j*deltaTime+tempoTotal(a)-tempoTotal(a)%deltaTime), p );
		}
		t = tempoTotal(a);
	}
	
	/* T1 */
    /*******************
    * Sessao critica!!!
    *******************/
	t = tempoTotal(a);
	transicao(a, T1);
	for( j=0; j<tempoTotal(a)/deltaTime-t/deltaTime; j++)
		atualizaPosicao( &tempoEspaco[t/deltaTime+j][a->id], a->id, (j*deltaTime+tempoTotal(a)-tempoTotal(a)%deltaTime), 100*NATAM );
	t = tempoTotal(a);



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
		for( j=0; j<tempoTotal(a)/deltaTime-t/deltaTime; j++) {
			p = 100*NATAM + 100.0*i + 100.0*( ( j*deltaTime + tempoTotal(a)-tempoTotal(a)%deltaTime)-t)/(tempoTotal(a)-t);
			atualizaPosicao( &tempoEspaco[t/deltaTime+j][a->id], a->id, (j*deltaTime+tempoTotal(a)-tempoTotal(a)%deltaTime), p );
		}
		t = tempoTotal(a);
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
	for( j=0; j<tempoTotal(a)/deltaTime-t/deltaTime; j++)
		atualizaPosicao( &tempoEspaco[t/deltaTime+j][a->id], a->id, (j*deltaTime+tempoTotal(a)-tempoTotal(a)%deltaTime), 100*NATAM + 1000*CITAM);
	t = tempoTotal(a);

	/**********************
    * Termino S. critica!!!
    **********************/
	/* Corrida */
	for(i=0; i<COTAM; i++){
		corrida(a);
		for( j=0; j<tempoTotal(a)/deltaTime-t/deltaTime; j++) {
			p = 100*NATAM + 1000*CITAM + 100.0*i + 100.0*( ( j*deltaTime + tempoTotal(a)-tempoTotal(a)%deltaTime)-t)/(tempoTotal(a)-t);
			atualizaPosicao( &tempoEspaco[t/deltaTime+j][a->id], a->id, (j*deltaTime+tempoTotal(a)-tempoTotal(a)%deltaTime), p );
		}
		t = tempoTotal(a);
	}
	/* Guardadndo tempo final na linha correspondente na matriz tempoEspaco. */
	for( i=tempoTotal(a)/deltaTime; i<(29*debug+1)*TMAX; i++)
		atualizaPosicao( &tempoEspaco[i][a->id], a->id, tempoTotal(a), 100*NATAM + 1000*CITAM + 1000*COTAM );

	/* SC */
	if(ticMax<tempoTotal(a)/deltaTime)
		ticMax = tempoTotal(a)/deltaTime;
	/* SC */

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
	deltaTime = 1800000;
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
	for( i=0; i<(29*debug+1)*TMAX; i++)
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
		atletas[i] = novoAtleta(FEMININO, PROFISSIONAL, randomName(M), randomName(S), i);
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
	for (; i<hp+mp+ha+ma; i++) {
		atletas[i] = novoAtleta(FEMININO, AMADOR, randomName(M), randomName(S), i);
		if (pthread_create(&(ids_atletas[i]),NULL, atleta, (void *) atletas[i] )) {
			fprintf(stderr,"Erro no pthread_create\n");
			return(2);
		}
	}

	for(i=0; i<natletas; i++) 
		pthread_join(ids_atletas[i],NULL); /* Esperara a junção das threads */
	
	pthread_join(id_classificacao,NULL);
	
	return 0;
}