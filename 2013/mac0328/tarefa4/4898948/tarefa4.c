 /*********************************************
 * Nome: Andre Menegheli Vale                 *
 * NUSP: 4898948                              *
 *                                            *
 * Tarefa 4                                   *
 *********************************************/
 
#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#define maxV 10
#define Vertex int
#define sp "  "
/* Mudar para 1 caso queira ver o resultado da busca em profundidade. */
#define Print 0

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
int dfsR( Digraph G, Vertex v) { 
   Vertex w;
   int r=1;
   ident++;
   /* lbl[v] = conta++; */
   conta++;
   for (w = 0; w < G->V; w++){
      if (G->adj[v][w] != 0){
         if(Print){
            printf("%d-%d",(int)v, (int)w);
            startSpace();
         }
         if (lbl[w] == -1){
            if(Print)
               printf(" dfsR(G,%d)", (int) w);
            lbl[w] = v;
            r = dfsR( G, w);
            if(r==-1) break;
         }
         else if(lbl[w]!=w){
            r = -1;
         }
      }
   }
   ident--;
   return r;
}

/* Função que atribui o valor val a todos os campos do vetor v. */
void resetArray(int *v, int n, int val){
   int i;
   for(i=0; i<n; i++)
      v[i] = val;
}

/* A função DIGRAPHdfs visita todos os vértices e todos os arcos do
 * digrafo G. A função registra a ordem em que os vértices são visitados
 * atribuindo um número de ordem lbl[x] a cada vértice x:  o k-ésimo
 * vértice visitado recebe número de ordem k-1.  (Código inspirado no
 * programa 18.3, p.87, de Sedgewick.) */
int DIGRAPHdfs( Digraph G) {
   Vertex v;
   conta = 0;
   ident = 0;
   resetArray(lbl, G->V, -1);
   for (v = 0; v < G->V; v++){
      if (lbl[v] == -1){
         if(Print)
            startSpace();
         if(Print)
            printf("dfsR(G,%d)", (int) v);
         if( dfsR( G, v)==-1 ) return -1;
      }
      if(conta==G->V)
         return v;
      conta = 0;
   }
   return -1;
}

/* A função abaixo imprime uma matriz com linhas 0..r-1 e colunas 0..c-1. */
void MATRIXprint( int r, int c, int **m) { 
   Vertex i, j;
   for (i = 0; i < r; i++){
      for (j = 0; j < c; j++)
         if(m[i][j]>=0)
            printf(" %2d", m[i][j]);
         else
            printf("  -");
      printf("\n");
   }
}

/* A função abaixo imprime um vetor com n posicoes. */
void parentPrint( int n, int *v) { 
   Vertex i;
   printf("pais:\n");
   for (i = 0; i < n; i++)
      printf(" %2d", i);
   printf("\n");
   for (i = 0; i < n; i++){
      if(v[i]>=0)
         printf(" %2d", v[i]);
      else
         printf("  -");
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

/* Esta função libera a memória usada pelo digrafo G. */
void DIGRAPHremove( Digraph G) { 
   int i;
   for(i=0;i<G->V;i++)
      free(G->adj[i]);
   free(G->adj);
   free(G);
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

/* Função que abre um arquivo com nome passado por str e retorna um Digrafo. */
Digraph DIGRAPHopenFile(char str[]){
   FILE *entra;
   Digraph D;
   int tam=0, i, j, kij;
   entra = fopen(str,"r");
   if( entra == NULL){
      printf("\nFalha na abertura do arquivo %s.",str);
      return NULL;
	}
   fscanf(entra,"%d",&tam);
   D = DIGRAPHinit(tam);
   for(i=0;i<tam;i++)
      for(j=0;j<tam;j++){
         fscanf(entra,"%d",&kij);
         if(kij==1)
            DIGRAPHinsertA( D, (Vertex)i, (Vertex)j);
      }
   fclose(entra);
   return D;
}

/* Funcao que inicializa as variaveis para realizar um teste. */
void teste( int n, char *files[]){
   Digraph G;
   int i, root;
   for(i=1;i<n;i++){
      G = DIGRAPHopenFile(files[i]);
      printf("\nAnaisando arquivo \"%s\"\n", files[i]);
      MATRIXprint( G->V, G->V, G->adj);
      lbl = malloc(G->V*sizeof(int));
      root = DIGRAPHdfs(G);
      if(root!=-1){
         printf("Arborecensia: sim\n");
         printf("raiz: %d\n", root);
         parentPrint(G->V, lbl);
         printf("\n");
      }
      else
         printf("Arborecensia: nao\n");
      DIGRAPHremove(G);
      free(lbl);
   }
}

int main(int argc, char *argv[]){
   if(argc==1)
      printf("Modo de uso: %s <arq. de teste1> <arq. de teste2> ... <arq. de testeN>\n", argv[0]);
   else
      teste(argc, argv);
   return 0;
}
