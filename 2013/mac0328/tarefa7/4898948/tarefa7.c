/******************************************************************************
# * Alunos: André Meneghelli Vale,      Núm. USP: 4898948
# * Curso: Bacharelado em Ciências da Computação
# * Tarefa 7 -- Calculo do diâmetro de um grafo.
# * MAC0328  -- 17/04/2013 -- IME/USP, -- Prof. Paulo Feofiloff
# * Compilador: gcc linux 4.6.3
# * Editor: Sublime Text 2;
# * Sistema Operacional: Linux
# *****************************************************************************/
 
#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#define Vertex int
#define sp "  "
#define INFINITO -1

struct digraph {
   int V; 
   int A; 
   int **adj;
};

/* Um objeto do tipo Digraph contém o endereço de um digraph. */
typedef struct digraph *Digraph;

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
Digraph DIGRAPHinit( int V, int A) { 
   Digraph G = malloc( sizeof *G);
   G->V = V; 
   G->A = A;
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
   if (v != w && G->adj[v][w] == 0)
      G->adj[v][w] = 1; 
}

/* Função que abre um arquivo com nome passado por str e retorna um Grafo. */
Digraph GRAPHopenFile(char str[]){
   FILE *entra;
   Digraph G;
   int tam=0, a, i, j;
   float f;
   entra = fopen(str,"r");
   if( entra == NULL){
      printf("\nFalha na abertura do arquivo %s.",str);
      return NULL;
   }
   fscanf(entra,"%d %d",&tam, &a);
   G = DIGRAPHinit(tam, a);
   while(!feof(entra)){
      fscanf(entra, "%d %d %f", &i, &j, &f);
      DIGRAPHinsertA( G, (Vertex)i, (Vertex)j);
      DIGRAPHinsertA( G, (Vertex)j, (Vertex)i);
   }
   fclose(entra);
   return G;
}

/* Armazena no vetor dist a distância do vértice s a cada um dos demais vértices do digrafo G: para cada vértice v, dist[v] é a distância de s a v.
 *(Código inspirado no programa 18.9, p.119, de Sedgewick.) */

int DIGRAPHdiametro( Digraph G) { 
   Vertex v, w;
   Vertex *pilha = (Vertex*) malloc(G->V*sizeof(Vertex));
   int maxD = 0;
   int p;
   int head = 0;
   int **dists = (int**) malloc(G->V*sizeof(int*));
   for (v = 0; v < G->V; v++){
      pilha[v] = -1;
      dists[v] = (int*) malloc(G->V*sizeof(int)); 
      for (w = 0; w < G->V; w++)
         dists[v][w] = INFINITO;
   }
   for (v = 0; v < G->V; v++) {
      pilha[head++]=v;
      dists[v][v] = 0;
      while (head) {
         p = pilha[--head];
         for (w = 0; w < G->V; w++) {
            if(w==p) continue;
            if ( G->adj[p][w]==1 && dists[v][w]==INFINITO) { 
               dists[v][w] = dists[v][p] + 1;
               dists[w][v] = dists[v][p] + 1;
               if( dists[v][p]+1 > maxD )
                  maxD = dists[v][p]+1;
               pilha[head++]=w;
            }
         }
      }
   }
   for (v = 0; v < G->V; v++)
      free(dists[v]);
   free(dists);
   free(pilha);
   return maxD;
}

/* Funcao que inicializa as variaveis para realizar um teste. */
void teste( int n, char *files[]){
   Digraph G;
   int i;
   for(i=1;i<n;i++){
      G = GRAPHopenFile(files[i]);
      printf("O diametro de \"%s\": %d\n", files[i], DIGRAPHdiametro(G) );
      DIGRAPHremove(G);
   }
}

int main(int argc, char *argv[]){
   if(argc==1)
      printf("Modo de uso: %s <arq. de teste1> <arq. de teste2> ... <arq. de testeN>\n", argv[0]);
   else
      teste(argc, argv);
   return 0;
}