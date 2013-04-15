#include "tmp.h"

/* Definindo as etapas. */
#define ETAPAS 5
#define NATACAO 0
#define T1 1
#define CICLISMO 2
#define T2 3
#define CORRIDA 4

/* Definindo sexo. */
#define MASCULINO 0
#define FEMININO 1

/* Definindo categoria. */
#define PROFISSIONAL 0
#define AMADOR 1

/* Definindo comportamento da estrada de ciclismo. */
#ifndef FAIXAS
	#define FAIXAS 3
#endif
#define SUBIDA 0
#define PLANO 1
#define DESCIDA 2

/* Definindo tamanho das provas. */
#define NATAM 38			/* x100m */ 
#define CITAM 180			/* x1km  */
#define COTAM 42 			/* x1km  */

/* Máximo de tempo possível para a prova em intervalos de meia hora. */
#define TMAX 53


/* Definindo se o programa rodará em segundos ou milissegundos (milissegundos por default). */
#ifndef PRECISAO
	#define PRECISAO 1
#endif

/* Brincando com cores! */
#define BLACK	30
#define RED		31
#define GREEN	32
#define YELLOW	33
#define BLUE	34
#define MAGENTA	35
#define CYAN	36
#define WHITE	37

/* Define para gerar uma simulação do algorítimo da punição. */
#ifdef SIMULACAO
	#define simulacao main
#else
	#define SIMULACAO 0
#endif

/* Tempo de punição. */
#ifndef TPUNI
	#define TPUNI 3
#endif

/* Defines para poder gerar nomes aleatórios.   */
/* Máximo de nomes encontrados nos dicionarios. */
#define MAXNAMES 10000
#define NMASCULINOS "nomes/homens.txt"
#define NFEMININOS "nomes/mulheres.txt"
#define SOBRENOMES "nomes/sobrenomes.txt"

/* Tempo em microssegundos para atrasar a impressão das linhas. */ 
#ifndef TIME
	#define TIME 0
#endif

typedef struct tmp{
    unsigned int h;			/* Hora */
	unsigned int m;			/* Minuto */
	unsigned int ms;		/* Segundo */
} *Tempo;

typedef struct atl{
	int id;
	char *nome;
	char *sobrenome;
	int sexo;
	int categoria;
	int ms[ETAPAS];			/* Tempo em milisegundos pra cada uma das etapas da prova */
} *Atleta;

typedef struct posAtleta{
	int id;				/* Id do atleta. */
	int ms;				/* Tempo em milissegundos. */
	double posicao;		/* Possição do atleta em metros. */
} PosicaoAtleta;

typedef struct listName
{
	unsigned int max;
	unsigned int prob[MAXNAMES];
	char nome[MAXNAMES][100];
} *ListName;

void *mallocX (unsigned int nbytes);
void *reallocX (void *ptr, unsigned int nbytes);
Tempo converteTempo(int ms);
Atleta novoAtleta(int sexo, int categoria, char *nome, char *sobrenome, int id);
PosicaoAtleta *novasPosicoes(int n);
void atualizaPosicao(PosicaoAtleta *p, int id, int t, double posicao);
int comparePosicaoAtleta( const void *p1, const void *p2);
void ordenaPosicaoAtleta( PosicaoAtleta *vet, int tam_vet);
int tempoTotal (Atleta a);
ListName listaNomes(char entrada[]);
char *randomName(ListName L);
int punicao(int *v, int tam, int tmp, int faixas);