#define ETAPAS 5
#define NATACAO 0
#define T1 1
#define CICLISMO 2
#define T2 3
#define CORRIDA 4

#define MASCULINO 0
#define FEMININO 1

#define PROFISSIONAL 0
#define AMADOR 1

#define SUBIDA 0
#define PLANO 1
#define DESCIDA 2

#define NATAM 38			/* x100m */ 
#define CITAM 180			/* x1km  */
#define COTAM 42 			/* x1km  */

typedef struct tmp{
    unsigned int h;			/* Hora */
	unsigned int m;			/* Minuto */
	unsigned int ms;		/* Segundo */
} *Tempo;

typedef struct atl{
	int id;
	int sexo;
	int categoria;
	int ms[ETAPAS];			/* Tempo em milisegundos pra cada uma das etapas da prova */
} *Atleta;

typedef struct posAtleta{
	int id;					/* Id do atleta. */
	double posicao;		/* Possição do atleta em metros. */
} PosicaoAtleta;

void *mallocX (unsigned int nbytes);
void *reallocX (void *ptr, unsigned int nbytes);
Tempo converteTempo(int ms);
Atleta novoAtleta(int sexo, int categoria, int id);
PosicaoAtleta *novasPossicoes(int n);
void atualizaPosicao(PosicaoAtleta *p, int id, double posicao);