#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h> /* Para trabalhar com threads */
#include <semaphore.h>
#include <string.h>
#include <unistd.h>

#include "tempos.h"

  /****************************************/
 /* Inicialização das variáveis globais. */
/****************************************/
int *TPortalT1Ent;
int *TPortalT2Ent;
int *TPortalT1Sai;
int *TPortalT2Sai;
int **estrada;
int *terrenos;

sem_t sem_PortalT1Ent;
sem_t sem_PortalT2Ent;
sem_t sem_PortalT1Sai;
sem_t sem_PortalT2Sai;
sem_t sem_estrada[CITAM];

/* Guarda a posição de cada um dos atletas como uma trinca entre o id, tempo e
 * espaço. Sendo que a primeira linha serve para guardar o tempo total de 
 * percurso e as demais linhas o tempo pra cada um dos tics que deve ser 
 * mostrado.
 *****************************************************************************/
PosicaoAtleta **tempoEspaco = NULL;

int natletas;
int deltaTime;
int debug;

Atleta *atletas;

  /*************************************************/
 /* Termino da declaração das variáveis globais.  */
/*************************************************/

/* Imprime a classificação dos atletas.
 **************************************/
void imprimeClassificacao(
	PosicaoAtleta *lista,		/* Lista dos tempos de cada atleta em uma determinada posição. */
	int tempo 					/* 1 caso queira imprimir os tempos e 0 caso seja a posição. */
){
	int i, j, tam, cor;
	Tempo t;
	char
		sex,
		cat,
		local[3];

	for( i=0; i<natletas; i++){
		if(!debug && i>=3) break;

		sex = atletas[lista[i].id]->sexo==MASCULINO ? 'M' : 'F';
		cat = atletas[lista[i].id]->categoria==PROFISSIONAL ? 'P' : 'A';

		if (lista[i].posicao < 100*NATAM)
			sprintf(local, "NA");
		else if (lista[i].posicao == 100*NATAM)
			sprintf(local, "TI");
		else if (lista[i].posicao < 100*NATAM + 1000*CITAM)
			sprintf(local, "CI");
		else if (lista[i].posicao == 100*NATAM + 1000*CITAM)
			sprintf(local, "T2");
		else if (lista[i].posicao < 100*NATAM + 1000*CITAM + 1000*COTAM)
			sprintf(local, "CO");
		else if (lista[i].posicao == 100*NATAM + 1000*CITAM + 1000*COTAM)
			sprintf(local, "FI");

		if(tempo) {
			cor = RED + atletas[lista[i].id]->sexo + 2 * atletas[lista[i].id]->categoria;
			t = converteTempo(lista[i].ms);
			printf("\033[%d;%dm%4d) \"%s %s\"", WHITE+10, cor, (i+1), atletas[lista[i].id]->nome, atletas[lista[i].id]->sobrenome);
			tam = strlen(atletas[lista[i].id]->nome)+strlen(atletas[lista[i].id]->sobrenome);
			for (j=0; j<22-tam; j++) {
				printf(" ");
			}
			if(PRECISAO==1)
				printf("%c   %c   %s   %02dh%02dm%02ds                \033[0m\n", sex, cat, local, t->h, t->m, t->ms/1000);
			else
				printf("%c   %c   %s   %02dh%02dm%06.3fs            \033[0m\n", sex, cat, local, t->h, t->m, (double)t->ms/1000);
			free(t);
		}
		else {
			cor = RED + atletas[lista[i].id]->sexo + 2 * atletas[lista[i].id]->categoria;
			printf("\033[%dm%4d) \"%s %s\"", cor,(i+1), atletas[lista[i].id]->nome, atletas[lista[i].id]->sobrenome);
			tam = log10(lista[i].posicao) + strlen(atletas[lista[i].id]->nome)+strlen(atletas[lista[i].id]->sobrenome);
			for (j=0; j<30-tam; j++) {
				printf(" ");
			}
			printf("%c   %c   %s   ", sex, cat, local);
			printf("%.2fm      \033[0m\n", lista[i].posicao);
		}
	}
}

/* Função usada na thread que imprime a classificação dos atletas a cada tic de 
 * tempo.
 *
 * Obs: esta função imprime uma nova linha somente quando todos os atletas
 * finalizam o tic. Não para cada grupo.
 ******************************************************************************/
void *classificacao( void *param ) {
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

	PosicaoAtleta **grupos=NULL;

	if(!debug){
		grupos = (PosicaoAtleta **) mallocX(4*sizeof(PosicaoAtleta *));
		for(i=0; i<4; i++)
			grupos[i] = novasPosicoes(natletas);
	}

	while( lPrint<(29*debug+1)*TMAX && ! (vmp && vfp && vma && vfa) ){
		if(TIME)
			usleep(TIME);

		for(i=0; i<natletas; i++)
			if( tempoEspaco[lPrint][i].id==-1) break;
		if(i==natletas){
			ordenaPosicaoAtleta( tempoEspaco[lPrint], natletas);
			if(debug) {
				for(i=0; i<natletas; i++)
					if( tempoEspaco[lPrint][i].posicao != 100*NATAM + 1000*CITAM + 1000*COTAM) break;
				if( i==natletas ) vmp = vfp = vma = vfa = 1;

				if(!vmp){
					printf("\n\033[%d;%dmClassificacao %dmin:                                                \033[0m\n", BLACK+10, WHITE, (29*!debug+1)*(lPrint+1));
					imprimeClassificacao(tempoEspaco[lPrint], 0);
				}
				else{
					printf(
						"\n\033[%d;%dm*********************************************************************\033[0m\n"
						"\033[%d;%dm**                      CLASSIFICACAO FINAL!!!                     **\033[0m\n"
						"\033[%d;%dm*********************************************************************\033[0m\n",
						BLACK, WHITE+10, BLACK, WHITE+10, BLACK, WHITE+10
					);
					imprimeClassificacao( tempoEspaco[lPrint], 1);
				}
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
				for(i=0; i<3 && i<mp && !vmp; i++)
					if( grupos[MASCULINO + 2 * PROFISSIONAL][i].posicao != 100*NATAM + 1000*CITAM + 1000*COTAM) break;
				if( (i==3 || i==mp) && !vmp )
					vmp=1;

				for(i=0; i<3 && i<fp && !vfp; i++)
					if( grupos[FEMININO + 2 * PROFISSIONAL][i].posicao != 100*NATAM + 1000*CITAM + 1000*COTAM) break;
				if( (i==3 || i==fp) && !vfp )
					vfp=1;

				for(i=0; i<3 && i<ma && !vma; i++)
					if( grupos[MASCULINO + 2 * AMADOR][i].posicao != 100*NATAM + 1000*CITAM + 1000*COTAM) break;
				if( (i==3 || i==ma) && !vma )
					vma=1;

				for(i=0; i<3 && i<fa && !vfa; i++)
					if( grupos[FEMININO + 2 * AMADOR][i].posicao != 100*NATAM + 1000*CITAM + 1000*COTAM) break;
				if( (i==3 || i==fa) && !vfa )
					vfa=1;

				/* Imprimindo a classificação ou finalização quando necessário pra cada um dos grupos. */
				printf("\n\033[%d;%dmClassificacao %dmin:                                                \033[0m\n", BLACK+10, WHITE, (29*!debug+1)*(lPrint+1));
				if(!vmp && mp)
					imprimeClassificacao(grupos[MASCULINO + 2 * PROFISSIONAL], 0);
				if(!vfp && fp)
					imprimeClassificacao(grupos[FEMININO + 2 * PROFISSIONAL], 0);
				if(!vma && ma)
					imprimeClassificacao(grupos[MASCULINO + 2 * AMADOR], 0);
				if(!vfa && fa)
					imprimeClassificacao(grupos[FEMININO + 2 * AMADOR], 0);
				
				if(vmp==1 && mp){
					printf(
						"\n\033[%d;%dm*********************************************************************\033[0m\n"
						"\033[%d;%dm**                      CLASSIFICACAO FINAL!!!                     **\033[0m\n"
						"\033[%d;%dm**-----------------------------------------------------------------**\033[0m\n"
						"\033[%d;%dm**                      Masculino Profissional                     **\033[0m\n"
						"\033[%d;%dm*********************************************************************\033[0m\n",
						BLACK, WHITE+10, BLACK, WHITE+10, BLACK, WHITE+10, BLACK, WHITE+10, BLACK, WHITE+10
					);
					imprimeClassificacao(grupos[MASCULINO + 2 * PROFISSIONAL], 1);
					vmp = 2;
				}

				if(vfp==1 && fp){
					printf(
						"\n\033[%d;%dm*********************************************************************\033[0m\n"
						"\033[%d;%dm**                      CLASSIFICACAO FINAL!!!                     **\033[0m\n"
						"\033[%d;%dm**-----------------------------------------------------------------**\033[0m\n"
						"\033[%d;%dm**                      Feminino  Profissional                     **\033[0m\n"
						"\033[%d;%dm*********************************************************************\033[0m\n",
						BLACK, WHITE+10, BLACK, WHITE+10, BLACK, WHITE+10, BLACK, WHITE+10, BLACK, WHITE+10
					);
					imprimeClassificacao(grupos[FEMININO + 2 * PROFISSIONAL], 1);
					vfp = 2;
				}

				if(vma==1 && ma){
					printf(
						"\n\033[%d;%dm*********************************************************************\033[0m\n"
						"\033[%d;%dm**                      CLASSIFICACAO FINAL!!!                     **\033[0m\n"
						"\033[%d;%dm**-----------------------------------------------------------------**\033[0m\n"
						"\033[%d;%dm**                         Masculino Amador                        **\033[0m\n"
						"\033[%d;%dm*********************************************************************\033[0m\n",
						BLACK, WHITE+10, BLACK, WHITE+10, BLACK, WHITE+10, BLACK, WHITE+10, BLACK, WHITE+10
					);
					imprimeClassificacao(grupos[MASCULINO + 2 * AMADOR], 1);
					vma = 2;
				}

				if(vfa==1 && fa){
					printf(
						"\n\033[%d;%dm*********************************************************************\033[0m\n"
						"\033[%d;%dm**                      CLASSIFICACAO FINAL!!!                     **\033[0m\n"
						"\033[%d;%dm**-----------------------------------------------------------------**\033[0m\n"
						"\033[%d;%dm**                          Feminino Amador                        **\033[0m\n"
						"\033[%d;%dm*********************************************************************\033[0m\n",
						BLACK, WHITE+10, BLACK, WHITE+10, BLACK, WHITE+10, BLACK, WHITE+10, BLACK, WHITE+10
					);
					imprimeClassificacao(grupos[FEMININO + 2 * AMADOR], 1);
					vfa = 2;
				}
			}
			lPrint++;
		}
	}

	if(!debug){
		for(i=0; i<4; i++)
			free(grupos[i]);
		free(grupos);
	}

	return NULL;
}

/* Função usada na thread atleta.
 ********************************/
void *atleta( void *param ){
	int i, j;
	int t=0;
	double p;
	Atleta a = (Atleta) param;

	/* Natação */
	for(i=0; i<NATAM; i++){
		natacao(a);
		for( j=0; j<tempoTotal(a)/deltaTime-t/deltaTime; j++) {
			p = 100.0*i + 100.0*( ( j*deltaTime + tempoTotal(a)-tempoTotal(a)%deltaTime)-t)/(tempoTotal(a)-t);
			atualizaPosicao( &tempoEspaco[t/deltaTime+j][a->id], a->id, (j*deltaTime+tempoTotal(a)-tempoTotal(a)%deltaTime), p );
		}
		t = tempoTotal(a);
	}
	
	t = tempoTotal(a);
	
	/* T1 */
	/* Passando pelo portal de entrada na troca da natação. */
	/* Sssão Crítica 1 */
	if(CONCORRENTE && !ITERATIVO) {
		sem_wait(&sem_PortalT1Ent);
		a->ms[NATACAO] += punicao( TPortalT1Ent, natletas, a->ms[NATACAO], 1);
		sem_post(&sem_PortalT1Ent);
	}
	else if(ITERATIVO)
		a->ms[NATACAO] += punicao( TPortalT1Ent, natletas, a->ms[NATACAO], 1);
	/* Término Sssão Crítica 1 */
	
	/* Tirando a sunga. */
	transicao(a, T1);
	
	/* Passando pelo portal de saida na troca da natação. */
	/* Sessão Crítica 2 */
	if(CONCORRENTE && !ITERATIVO) {
		sem_wait(&sem_PortalT1Sai);
		a->ms[T1] += punicao( TPortalT1Sai, natletas, a->ms[T1], 1);
		sem_post(&sem_PortalT1Sai);
	}
	else if(ITERATIVO)
		a->ms[T1] += punicao( TPortalT1Sai, natletas, a->ms[T1], 1);
	/* Término Sessão Crítica 2 */
	
	/* Bufferizando o tempo em que o atleta esteve parado na troca 1. */
	for( j=0; j<tempoTotal(a)/deltaTime-t/deltaTime; j++)
		atualizaPosicao( &tempoEspaco[t/deltaTime+j][a->id], a->id, (j*deltaTime+tempoTotal(a)-tempoTotal(a)%deltaTime), 100*NATAM );

	t = tempoTotal(a);
	/* Ciclismo */
	for(i=0; i<CITAM; i++){
		ciclismo(a, terrenos[i]);
		for( j=0; j<tempoTotal(a)/deltaTime-t/deltaTime; j++) {
			p = 100*NATAM + 1000.0*i + 1000.0*( ( j*deltaTime + tempoTotal(a)-tempoTotal(a)%deltaTime)-t)/(tempoTotal(a)-t);

			/* Sessão Crítica 3 */
			if(CONCORRENTE && !ITERATIVO) {
				sem_wait(&sem_estrada[i]);
				a->ms[CICLISMO] += punicao( estrada[i], natletas, a->ms[CICLISMO], 3);
				sem_post(&sem_estrada[i]);
			}
			else if(ITERATIVO)
				a->ms[CICLISMO] += punicao( estrada[i], natletas, a->ms[CICLISMO], 3);
			/* Término Sessão Crítica 3 */

			atualizaPosicao( &tempoEspaco[t/deltaTime+j][a->id], a->id, (j*deltaTime+tempoTotal(a)-tempoTotal(a)%deltaTime), p );
		}
		t = tempoTotal(a);
	}	

	/* T2 */
	t = tempoTotal(a);
	
	/* Sessão Crítica 4 */
	if(CONCORRENTE && !ITERATIVO) {
		sem_wait(&sem_PortalT2Ent);
		a->ms[CICLISMO] += punicao( TPortalT2Ent, natletas, a->ms[CICLISMO], 1);
		sem_post(&sem_PortalT2Ent);
	}
	else if(ITERATIVO)
		a->ms[CICLISMO] += punicao( TPortalT2Ent, natletas, a->ms[CICLISMO], 1);
	/* Término Sessão Crítica 4 */

	/* Trocando o sapato. */
	transicao(a, T2);

	/* Sessão Crítica 5 */
	if(CONCORRENTE && !ITERATIVO) {
		sem_wait(&sem_PortalT2Sai);
		a->ms[T2] += punicao( TPortalT2Sai, natletas, a->ms[T2], 1);
		sem_post(&sem_PortalT2Sai);
	}
	else if(ITERATIVO)
		a->ms[T2] += punicao( TPortalT2Sai, natletas, a->ms[T2], 1);
	/* Término Sessão Crítica 5 */
	
	for( j=0; j<tempoTotal(a)/deltaTime-t/deltaTime; j++)
		atualizaPosicao( &tempoEspaco[t/deltaTime+j][a->id], a->id, (j*deltaTime+tempoTotal(a)-tempoTotal(a)%deltaTime), 100*NATAM + 1000*CITAM);
	t = tempoTotal(a);

	/* Corrida */
	for(i=0; i<COTAM; i++){
		corrida(a);
		for( j=0; j<tempoTotal(a)/deltaTime-t/deltaTime; j++) {
			p = 100*NATAM + 1000*CITAM + 1000.0*i + 1000.0*( ( j*deltaTime + tempoTotal(a)-tempoTotal(a)%deltaTime)-t)/(tempoTotal(a)-t);
			atualizaPosicao( &tempoEspaco[t/deltaTime+j][a->id], a->id, (j*deltaTime+tempoTotal(a)-tempoTotal(a)%deltaTime), p );
		}
		t = tempoTotal(a);
	}
	/* Guardadndo tempo final na linha correspondente na matriz tempoEspaco. */
	for( i=tempoTotal(a)/deltaTime; i<(29*debug+1)*TMAX; i++)
		atualizaPosicao( &tempoEspaco[i][a->id], a->id, tempoTotal(a), 100*NATAM + 1000*CITAM + 1000*COTAM );

	return NULL;
}

/* Função principal.
 *******************/
int ironMain(int argc, char *argv[]){
	char
		*filename = NULL,
		terreno;
	FILE *entrada;
	int
		i,
		j,
		t,
		hp,
		mp,
		ha,
		ma;

	ListName H = listaNomes(NMASCULINOS);
	ListName M = listaNomes(NFEMININOS);
	ListName S = listaNomes(SOBRENOMES);
	
	pthread_t *ids_atletas;
	pthread_t id_classificacao;
	
	srand( time(NULL) );

	/* Inicializando variáveis globais. */
	tempoEspaco = NULL;	
	deltaTime = 1800;
	debug = 0;

	/* Verificando os parametros de entrada. */
	for( i=1; i<argc; i++){
		if( strcmp(argv[i], "-debug")==0 ){
			deltaTime = 60;
			debug=1;
		}
		else
			filename = argv[i];
	}

	deltaTime *= PRECISAO;

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
	if ( fscanf(entrada, "%d\n%d\n%d\n%d", &hp, &mp, &ha, &ma) != 4 ) {
		printf("Falha ao carregar o arquivo %s.\n", filename);
		return 1;
	}
	terrenos = (int *)mallocX(CITAM*sizeof(int));
	i=0;
	while(!feof(entrada)){
		if ( fscanf(entrada, "%c %d", &terreno, &t) ){
			printf("Falha ao carregar o arquivo %s.\n", filename);
			return 1;
		}
		if(terreno=='S')
			for( j=0; j<t; j++, i++ )
				terrenos[i] = SUBIDA;
		if(terreno=='P')
			for( j=0; j<t; j++, i++ )
				terrenos[i] = PLANO;
		if(terreno=='D')
			for( j=0; j<t; j++, i++ )
				terrenos[i] = DESCIDA;
	}
	fclose(entrada);

	natletas = hp + mp + ha + ma;

	/* Alocando a quantidade máxima de tics necessária para guardar as informações de tempo dos atletas. */
	tempoEspaco = (PosicaoAtleta **) mallocX( (29*debug+1)*TMAX*sizeof(PosicaoAtleta*) );
	for( i=0; i<(29*debug+1)*TMAX; i++)
		tempoEspaco[i] = novasPosicoes(natletas);

	if(CONCORRENTE) {
		TPortalT1Ent = (int*) mallocX( natletas*sizeof(int) );
		TPortalT2Ent = (int*) mallocX( natletas*sizeof(int) );
		TPortalT1Sai = (int*) mallocX( natletas*sizeof(int) );
		TPortalT2Sai = (int*) mallocX( natletas*sizeof(int) );
		estrada = (int**) mallocX( CITAM*sizeof(int *) );

		sem_init(&sem_PortalT1Ent, 0, 1);
		sem_init(&sem_PortalT1Sai, 0, 1);
		sem_init(&sem_PortalT2Ent, 0, 1);
		sem_init(&sem_PortalT2Sai, 0, 1);

		for( i=0; i<CITAM; i++){
			estrada[i] = (int*) mallocX( natletas*sizeof(int) );
			sem_init(&sem_estrada[i], 0, 1);
			for( j=0; j<natletas; j++){
				estrada[i][j] = -1;
				TPortalT1Ent[j] = -1;
				TPortalT2Ent[j] = -1;
				TPortalT1Sai[j] = -1;
				TPortalT2Sai[j] = -1;
			}
		}
	}

	ids_atletas = mallocX(natletas*sizeof(pthread_t));
	atletas = (Atleta*) mallocX(natletas*sizeof(Atleta));

	pthread_create(&(id_classificacao),NULL,  classificacao, NULL);

	if(CONCORRENTE)
		printf("Solucao concorrente.\n");
	else
		printf("Solucao paralela.\n");

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

	/* Threads terminadas, agora vamos liberar a memória. */
	
	if(CONCORRENTE) {
		sem_destroy(&sem_PortalT1Ent);
		sem_destroy(&sem_PortalT1Sai);
		sem_destroy(&sem_PortalT2Ent);
		sem_destroy(&sem_PortalT2Sai);
		
		for(i=0; i<natletas; i++) 
			sem_destroy(&sem_estrada[i]);

		for( i=0; i<CITAM; i++)
			free(estrada[i]);
	
		free(TPortalT1Ent);
		free(TPortalT2Ent);
		free(TPortalT1Sai);
		free(TPortalT2Sai);
		free(estrada);
	}
	
	for(i=0; i<(29*debug+1)*TMAX; i++)
		free(tempoEspaco[i]);

	free(tempoEspaco);
	free(ids_atletas);
	free(atletas);
	free(terrenos);
	free(H);
	free(M);
	free(S);
	
	return 0;
}

int gutsMain(int argc, char *argv[]){
	char
		*filename = NULL,
		terreno;
	FILE *entrada;
	int
		i,
		j,
		t,
		hp,
		mp,
		ha,
		ma;

	ListName H = listaNomes(NMASCULINOS);
	ListName M = listaNomes(NFEMININOS);
	ListName S = listaNomes(SOBRENOMES);
	
	srand( time(NULL) );

	/* Inicializando variáveis globais. */
	tempoEspaco = NULL;	
	deltaTime = 1800;
	debug = 0;

	/* Verificando os parametros de entrada. */
	for( i=1; i<argc; i++){
		if( strcmp(argv[i], "-debug")==0 ){
			deltaTime = 60;
			debug=1;
		}
		else
			filename = argv[i];
	}

	deltaTime *= PRECISAO;

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
	if( fscanf(entrada, "%d\n%d\n%d\n%d", &hp, &mp, &ha, &ma) != 4 ){
		printf("Falha ao ler arquivo %s.\n", filename);
		return 1;
	}
	terrenos = (int *)mallocX(CITAM*sizeof(int));
	i=0;
	while(!feof(entrada)){
		if( fscanf(entrada, "%c %d", &terreno, &t) ){
			printf("Falha ao ler arquivo %s.\n", filename);
			return 1;
		}
		if(terreno=='S')
			for( j=0; j<t; j++, i++ )
				terrenos[i] = SUBIDA;
		if(terreno=='P')
			for( j=0; j<t; j++, i++ )
				terrenos[i] = PLANO;
		if(terreno=='D')
			for( j=0; j<t; j++, i++ )
				terrenos[i] = DESCIDA;
	}
	fclose(entrada);

	natletas = hp + mp + ha + ma;

	TPortalT1Ent = (int*) mallocX( natletas*sizeof(int) );
	TPortalT2Ent = (int*) mallocX( natletas*sizeof(int) );
	TPortalT1Sai = (int*) mallocX( natletas*sizeof(int) );
	TPortalT2Sai = (int*) mallocX( natletas*sizeof(int) );
	estrada = (int**) mallocX( CITAM*sizeof(int *) );

	for( i=0; i<CITAM; i++){
		estrada[i] = (int*) mallocX( natletas*sizeof(int) );
		for( j=0; j<natletas; j++){
			estrada[i][j] = -1;
			TPortalT1Ent[j] = -1;
			TPortalT2Ent[j] = -1;
			TPortalT1Sai[j] = -1;
			TPortalT2Sai[j] = -1;
		}
	}

	/* Alocando a quantidade máxima de tics necessária para guardar as informações de tempo dos atletas. */
	tempoEspaco = (PosicaoAtleta **) mallocX( (29*debug+1)*TMAX*sizeof(PosicaoAtleta*) );
	for( i=0; i<(29*debug+1)*TMAX; i++)
		tempoEspaco[i] = novasPosicoes(natletas);

	atletas = (Atleta*) mallocX(natletas*sizeof(Atleta));

	printf("Solucao Iterativa.\n");
	printf("Criando atletas: \n");
	printf("\tMasculino Profissional: %d\n", hp);
	for (i=0; i<hp; i++) {
		atletas[i] = novoAtleta(MASCULINO, PROFISSIONAL, randomName(H), randomName(S), i);
		atleta(atletas[i]);
	}

	printf("\tFeminino Profissional: %d\n", mp);
	for (; i<hp+mp; i++) {
		atletas[i] = novoAtleta(FEMININO, PROFISSIONAL, randomName(M), randomName(S), i);
		atleta(atletas[i]);
	}

	printf("\tMasculino Amador: %d\n", ha);
	for (; i<hp+mp+ha; i++) {
		atletas[i] = novoAtleta(MASCULINO, AMADOR, randomName(H), randomName(S), i);
		atleta(atletas[i]);
	}

	printf("\tFeminino Amador: %d\n", ma);
	for (; i<hp+mp+ha+ma; i++) {
		atletas[i] = novoAtleta(FEMININO, AMADOR, randomName(M), randomName(S), i);
		atleta(atletas[i]);
	}

	classificacao(NULL);
	
	for( i=0; i<CITAM; i++)
		free(estrada[i]);

	free(TPortalT1Ent);
	free(TPortalT2Ent);
	free(TPortalT1Sai);
	free(TPortalT2Sai);
	free(estrada);

	for(i=0; i<(29*debug+1)*TMAX; i++)
		free(tempoEspaco[i]);

	free(tempoEspaco);
	free(atletas);
	free(terrenos);
	free(H);
	free(M);
	free(S);

	return 0;
}