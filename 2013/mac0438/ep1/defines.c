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
	
	t->ms = ms%(60*(int)PRECISAO);
	t->m = ms%(3600*(int)PRECISAO)/(60*(int)PRECISAO);
	t->h = ms/(3600*(int)PRECISAO);
	
	if(PRECISAO==1)
		t->ms *= 1000;
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

PosicaoAtleta *novasPosicoes(int n){
	int i;
	PosicaoAtleta *p = (PosicaoAtleta *) mallocX(n*sizeof(PosicaoAtleta));

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
   qsort( vet, tam_vet, sizeof (PosicaoAtleta), comparePosicaoAtleta);
}

ListName listaNomes(char entrada[]){
	FILE *fe = fopen(entrada, "r");
	char buffer[100];
	ListName lista = (ListName) mallocX(sizeof(struct listName));
	int i, k;

	lista->max = 0;

	for( i=0; !feof(fe) && i<MAXNAMES ; i++){
		if ( fgets( buffer, 100, fe)==NULL )
			break;
		if( sscanf(buffer, "%s %d", lista->nome[i], &k) != 2 )
			break;
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

/* Função interna para ajudar a imprimir a simulação do algoritmo. */
void print2Spc(int tam, char str[]){
	int i;

	for(i=0; i<tam; i++)
		printf("    ");
	printf("%s", str);
}

/* Função interna recursiva que calcula o tempo de punição pra cada atleta. (Pode ser simulada). */
int punicaoR(
	int *v,			/* Vetor que representa o tempo de um atleta em uma posição. (Deve ser inicializado com -1). */
	int ini,		/* Início do setor de comparação. */
	int fim,		/* Fim do setor de comparação. */
	int tam,		/* Tamanho total do vetor. (Necessário em alguns casos de punição). */
	int tmp,		/* Tempo que esta tentando inserir na posição definida por v. */
	int faixas 		/* Número de faixas disponíveis na via. */
){
	int
		esq=0,
		dir=0,
		i,
		m = (ini+fim)/2,
		mem;

	/* Caso seja simulação, imprimir a recursão adequadamente. */
	if(SIMULACAO){
		char str[5];
		sprintf(str, "%3d ", tmp);
		print2Spc(m, str );
		printf("\n");
	}

	if (v[m]==tmp){
		/* Verificando ocupação das vias adjacentes. */
		for( i=1; i<faixas && faixas+i<tam; i++, dir++)
			if(v[m+i]!=tmp) break;
		for( i=1; i<faixas && faixas-i>0; i++, esq++)
			if(v[m-i]!=tmp) break;

		/* Caso o atleta esteja tentando ultrapassar alguém sem ter uma via, será punido!!! */
		if(dir+esq+2>faixas) { /* uso + 2 pois estou tentando 'usar' uma via a mais do que a que já esta sendo utilizada*/
			
			/* Todos os tempos são menores que o do atleta atual. Memória deve ser tratada no subconjunto onde o valor punido é possível de ser encontrado. */
			if(m+dir+1<tam) {
				mem = punicaoR(
					v,
					m+dir+1, /* Definindo posição inicial do subconjunto */
					( m+dir+1+TPUNI*PRECISAO*faixas<tam ? m+dir+TPUNI*PRECISAO*faixas : tam-1), /* Definindo posição final do subconjunto. */
					tam,
					tmp+TPUNI*PRECISAO,
					faixas
				);
				if(mem==-1) return -1;
				return TPUNI*PRECISAO + mem;
			}
			else{
				memcpy ( v, v+1, (tam-1)*sizeof(int) );
				v[tam-1] = tmp+TPUNI;
				return TPUNI*PRECISAO;
			}
		}
		else{
			memcpy ( v, v+1, m*sizeof(int) );
			v[m] = tmp;
			return 0;
		}
	}
	else if( ini==fim && v[ini]!=tmp ){
		if (v[ini]<tmp) {
			memcpy ( v, v+1, ini*sizeof(int) );
			v[ini] = tmp;
		}
		else if(v[ini]>tmp) {
			if(ini==0)
				return -1;
			memcpy ( v, v+1, (ini)*sizeof(int) );
			v[ini-1] = tmp;
		}
		return 0;
	}
	else if ( v[m]<tmp ){
		mem = punicaoR(v, m+1, fim, tam, tmp, faixas);
		if(mem==-1) return -1;
		return mem;
	}
	else{
		mem = punicaoR(v, ini, m, tam, tmp, faixas);
		if(mem==-1) return -1;
		return mem;
	}

	/* Aqui os bits já estão bem lustrosos. */
}

/* Definindo o primeiro conjunto de busca. */
int punicao(int *v, int tam, int tmp, int faixas){
	return punicaoR(v, 0, tam-1, tam, tmp, faixas);
}

int simulacao(int argc, char *argv[]){
	int
		i,
		j,
		p,
		mem,
		faixas=1,
		nat=40,
		*v;
	char c;

	srand( time(NULL) );

	for(i=1; i<argc; i++){
		if ( !strcmp(argv[i], "-h") ){
			printf(
				"Modo de uso:\n"
				"%s -h <faixas> <atletas>\n"
				"-h: para imprimir esta ajuda.\n"
				"<faixas>: numero de faixas disponiveis.\n"
				"<atletas>: numero de atletas.\n", argv[0]
			);
			return 0;
		}
		if(i==1)
			faixas = atoi(argv[i]);
		else if(i==2)
			nat = atoi(argv[i]);
	}

	v = (int *) mallocX(nat*sizeof(int));

	for ( i=0; i<nat; ++i)
	{
		v[i]=-1;
	}

	i=0;
	while(1){
		if ( system("clear")==-1 ){
			printf("Falha ao limpar a tela.\n");
		}

		printf("\n\nSimulacao %d\n", i+1);
		mem = rand()%5;
		printf("Inserindo %d\n\n", mem);

		for( j=0; j<nat; j++)
			printf("%3d ", j);
		printf("\n");
		for( j=0; j<nat; j++){
			if( v[j]==-1 )
				printf("  - ");
			else
				printf("%3d ", v[j]);
		}
		p = punicao(v, nat, mem, faixas);
		for( j=0; j<nat; j++)
			printf("%3d ", j);
		printf("\n");
		for( j=0; j<nat; j++){
			if( v[j]==-1 )
				printf("  - ");
			else
				printf("%3d ", v[j]);
		}
		printf("\n\nPunicao = %d\nValor inserido = %d\n\nDeseja continuar? [Y/q]: ", p, p+mem );

		c = getchar();

		if(c=='q')
			break;
		i++;
	}

	free(v);

	return 0;
}