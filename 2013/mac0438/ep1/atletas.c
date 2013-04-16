#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h> /* Para trabalhar com threads */
#include <string.h>
#include <unistd.h>

#include "defines.h"


/* Função que calcula um tempo aleatório que tem valor minimo de min e máximo de max. */
int timeRand(int min, int max){
	return (PRECISAO*min + PRECISAO*rand()/RAND_MAX*(max-min));
}

/* As funções a seguir calculam o tempo gasto em cada etapa para um Atleta a.	*
*  Dependendo da etapa deve ser passado um parametro extra para a função. 		*/
void natacao(Atleta a){
	if( a->sexo == FEMININO && a->categoria == AMADOR )
		a->ms[NATACAO] += timeRand(150, 300);
	if( a->sexo == MASCULINO && a->categoria == AMADOR )
		a->ms[NATACAO] += timeRand(120, 240);
	if( a->sexo == FEMININO && a->categoria == PROFISSIONAL )
		a->ms[NATACAO] += timeRand(90, 120);
	if( a->sexo == MASCULINO && a->categoria == PROFISSIONAL )
		a->ms[NATACAO] += timeRand(75, 100);
}

void corrida(Atleta a){
	if( a->sexo == FEMININO && a->categoria == AMADOR )
		a->ms[NATACAO] += timeRand(330, 420);
	if( a->sexo == MASCULINO && a->categoria == AMADOR )
		a->ms[NATACAO] += timeRand(300, 420);
	if( a->sexo == FEMININO && a->categoria == PROFISSIONAL )
		a->ms[NATACAO] += timeRand(260, 290);
	if( a->sexo == MASCULINO && a->categoria == PROFISSIONAL )
		a->ms[NATACAO] += timeRand(240, 260);
}

/* Para esta função o tipo de terreno também deve ser passado como parametro de entrada. */
void ciclismo( Atleta a, int terreno){
	switch(terreno){
		case SUBIDA:
			if( a->sexo == FEMININO && a->categoria == AMADOR )
				a->ms[CICLISMO] += timeRand(3600/10, 3600/15);
			if( a->sexo == MASCULINO && a->categoria == AMADOR )
				a->ms[CICLISMO] += timeRand(3600/12, 3600/20);
			if( a->sexo == FEMININO && a->categoria == PROFISSIONAL )
				a->ms[CICLISMO] += timeRand(3600/20, 3600/30);
			if( a->sexo == MASCULINO && a->categoria == PROFISSIONAL )
				a->ms[CICLISMO] += timeRand(3600/30, 3600/40);
			break;
		case PLANO:
			if( a->sexo == FEMININO && a->categoria == AMADOR )
				a->ms[CICLISMO] += timeRand(3600/25, 3600/40);
			if( a->sexo == MASCULINO && a->categoria == AMADOR )
				a->ms[CICLISMO] += timeRand(3600/30, 3600/45);
			if( a->sexo == FEMININO && a->categoria == PROFISSIONAL )
				a->ms[CICLISMO] += timeRand(3600/45, 3600/50);
			if( a->sexo == MASCULINO && a->categoria == PROFISSIONAL )
				a->ms[CICLISMO] += timeRand(3600/50, 3600/55);
			break;
		case DESCIDA:
			if( a->sexo == FEMININO && a->categoria == AMADOR )
				a->ms[CICLISMO] += timeRand(3600/50, 3600/65);
			if( a->sexo == MASCULINO && a->categoria == AMADOR )
				a->ms[CICLISMO] += timeRand(3600/50, 3600/70);
			if( a->sexo == FEMININO && a->categoria == PROFISSIONAL )
				a->ms[CICLISMO] += timeRand(3600/70, 3600/90);
			if( a->sexo == MASCULINO && a->categoria == PROFISSIONAL )
				a->ms[CICLISMO] += timeRand(3600/100, 3600/100);
			break;
	}
}

/* Para esta função é preciso especificar o numero da transicao no segundo parametro. */
void transicao( Atleta a, int t){
	if(t==T1){
		if( a->sexo == FEMININO && a->categoria == AMADOR )
			a->ms[T1] += timeRand(150, 390);
		if( a->sexo == MASCULINO && a->categoria == AMADOR )
			a->ms[T1] += timeRand(150, 330);
		if( a->sexo == FEMININO && a->categoria == PROFISSIONAL )
			a->ms[T1] += timeRand(90, 150);
		if( a->sexo == MASCULINO && a->categoria == PROFISSIONAL )
			a->ms[T1] += timeRand(90, 150);
	}
	else{
		if( a->sexo == FEMININO && a->categoria == AMADOR )
			a->ms[T2] += timeRand(270, 720);
		if( a->sexo == MASCULINO && a->categoria == AMADOR )
			a->ms[T2] += timeRand(240, 600);
		if( a->sexo == FEMININO && a->categoria == PROFISSIONAL )
			a->ms[T2] += timeRand(150, 360);
		if( a->sexo == MASCULINO && a->categoria == PROFISSIONAL )
			a->ms[T2] += timeRand(120, 180);
	}
}

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
	if(CONCORRENTE) {
		sem_wait(&sem_PortalT1Ent);
		a->ms[NATACAO] += punicao( TPortalT1Ent, natletas, a->ms[NATACAO], 1);
		sem_post(&sem_PortalT1Ent);
	}
	/* Término Sssão Crítica 1 */
	
	/* Tirando a sunga. */
	transicao(a, T1);
	
	/* Passando pelo portal de saida na troca da natação. */
	/* Sessão Crítica 2 */
	if(CONCORRENTE) {
		sem_wait(&sem_PortalT1Sai);
		a->ms[T1] += punicao( TPortalT1Sai, natletas, a->ms[T1], 1);
		sem_post(&sem_PortalT1Sai);
	}
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
			if(CONCORRENTE) {
				sem_wait(&sem_estrada[i]);
				a->ms[CICLISMO] += punicao( estrada[i], natletas, a->ms[CICLISMO], 3);
				sem_post(&sem_estrada[i]);
			}
			/* Término Sessão Crítica 3 */

			atualizaPosicao( &tempoEspaco[t/deltaTime+j][a->id], a->id, (j*deltaTime+tempoTotal(a)-tempoTotal(a)%deltaTime), p );
		}
		t = tempoTotal(a);
	}	

	/* T2 */
	t = tempoTotal(a);
	
	/* Sessão Crítica 4 */
	if(CONCORRENTE) {
		sem_wait(&sem_PortalT2Ent);
		a->ms[CICLISMO] += punicao( TPortalT2Ent, natletas, a->ms[CICLISMO], 1);
		sem_post(&sem_PortalT2Ent);
	}
	/* Término Sessão Crítica 4 */

	/* Trocando o sapato. */
	transicao(a, T2);

	/* Sessão Crítica 5 */
	if(CONCORRENTE) {
		sem_wait(&sem_PortalT2Sai);
		a->ms[T2] += punicao( TPortalT2Sai, natletas, a->ms[T2], 1);
		sem_post(&sem_PortalT2Sai);
	}
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