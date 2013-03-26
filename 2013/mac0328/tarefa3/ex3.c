 /*********************************************
 * Nome: Andre Menegheli Vale                 *
 * NUSP: 4898948                              *
 *                                            *
 * Tarefa 3                                   *
 * Exercicio 3                                *
 *********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define maxV 10
#define Vertex int
#define sp "  "

struct digraph {
   int V; 
   int A; 
   int **adj; 
};

/* Um objeto do tipo Digraph contém o endereço de um digraph. */
typedef struct digraph *Digraph;

static int conta, *lbl, ident;

/* Imprime os espacos de identacao */
void startSpace( void ){
    int i;
    printf("\n");
    for(i=0; i<ident;i++)
        printf(sp);
}

/* A função dfsR supõe que o digrafo G é representado por uma matriz de
 * adjacência.  (Inspirado no programas 18.1, p.82, de Sedgewick.) */
void dfsR( Digraph G, Vertex v) { 
    Vertex w;
    ident++;
    lbl[v] = conta++; 
    for (w = 0; w < G->V; w++){
        if (G->adj[v][w] != 0){
            startSpace();
            printf("%d-%d",(int)v, (int)w);
            if (lbl[w] == -1){
                printf(" dfsR(G,%d)", (int) w);
                dfsR( G, w);
            }
        }
    }
    ident--;
}

/* A função DIGRAPHdfs visita todos os vértices e todos os arcos do
 * digrafo G. A função registra a ordem em que os vértices são visitados
 * atribuindo um número de ordem lbl[x] a cada vértice x:  o k-ésimo
 * vértice visitado recebe número de ordem k-1.  (Código inspirado no
 * programa 18.3, p.87, de Sedgewick.) */
void DIGRAPHdfs( Digraph G) {
    Vertex v;
    conta = 0;
    ident = 0;
    for (v = 0; v < G->V; v++) 
        lbl[v] = -1;
    for (v = 0; v < G->V; v++){
        if (lbl[v] == -1){
            startSpace();
            printf("dfsR(G,%d)", (int) v);
            dfsR( G, v);
       }
    }
}

/* A função abaixo aloca uma matriz com linhas 0..r-1 e colunas 0..c-1.
 * Cada elemento da matriz recebe valor val. */
int **MATRIXint( int r, int c, int val) { 
   Vertex i, j;
   int **m = malloc( r * sizeof (int *));
   for (i = 0; i < r; i++)
      m[i] = malloc( c * sizeof (int));
   for (i = 0; i < r; i++)
      for (j = 0; j < c; j++)
         m[i][j] = val;
   return m;
}

/* Esta função devolve (o endereço de) um novo digrafo com vértices
 * 0,..,V-1 e nenhum arco. */
Digraph DIGRAPHinit( int V) { 
   Digraph G = malloc( sizeof *G);
   G->V = V; 
   G->A = 0;
   G->adj = MATRIXint( V, V, 0);
   return G;
}

/* Esta função insere um arco v-w no digrafo G. Se v == w ou o digrafo
 * já tem arco v-w, a função não faz nada.  É claro que v e w não podem
 * ser negativos e devem ser menores que G->V. */
void DIGRAPHinsertA( Digraph G, Vertex v, Vertex w) { 
   if (v != w && G->adj[v][w] == 0) {
      G->adj[v][w] = 1; 
      G->A++;
   }
}

/* Esta função remove do digrafo G o arco que tem ponta inicial v e
 * ponta final w.  Se não existe tal arco, a função nada faz. */
void DIGRAPHremoveA( Digraph G, Vertex v, Vertex w) { 
   if (G->adj[v][w] == 1) {
      G->adj[v][w] = 0; 
      G->A--;
   }
}

/* Para cada vértice v do digrafo G, esta função imprime, em uma linha,
 * todos os vértices adjacentes a v. */
void DIGRAPHshow( Digraph G) { 
   Vertex v, w; 
   for (v = 0; v < G->V; v++) {
      printf( "%2d:", v);
      for (w = 0; w < G->V; w++)
         if (G->adj[v][w] == 1) 
            printf( " %2d", w);
      printf( "\n");
   }
}

/* Funcao que inicializa um digrafo G com valores aleatorios. Sendo
 * prob a probabilidade de existencia de uma aresta. */
void DIGRAPHrand( Digraph G, int prob){
    int i, j;
    srand(time(NULL));
    for(i=0; i<G->V; i++){
        for(j=0; j<G->V; j++){
            if(i==j) continue;
            G->adj[i][j] = (rand()%100>prob)?0:1;
        }
    }
}

/* Funcao que inicializa as variaveis para realizar um teste. */
void teste( int tam){
    Digraph G;
    lbl = malloc(tam*sizeof(int));
    G = DIGRAPHinit(tam);
    DIGRAPHrand( G, 20);
    DIGRAPHshow(G);
    DIGRAPHdfs(G);
}

/* A funcao principal pega o primeiro parametro extra da linha de
 * comando para o tamanho maximo de vertices ou admite o valor como
 * sendo maxV. */
int main(int argc, char *argv[]){
    int tam=maxV;
    if(argc==1){
        printf("Modo de uso: %s <n>\n", argv[0]);
        printf("Onde <n>: numero de vertices.\n");
        printf("Caso <n> nao exista admite-se que <n> seja %d.\n\n", maxV);
    }
    else if(argc>1)
        sscanf(argv[1],"%d",&tam);
    teste(tam);
    printf("\n\n");
    return 0;
}
