/*
 * ep1.c
 *
 *  Created on: Apr 13, 2016
 *      Author: mcello
 */

/*******************************************************************************
 * MAC0327 -- IME/USP
 * Aluno: Marcello Souza de Oliveira, NUSP 6432692
 *        André Vale,                 NUSP 4898948
 * Curso: Bacharelado em Ciencias da Computacao
 * Compilador: g++ linux 4.9.2
 * Opcoes compilacao: -Wall -ansi -pedantic -O2 -U_FORTIFY_SOURCE -lm
 * Compilar:
 *  g++ -Wall -ansi -pedantic -O2 -U_FORTIFY_SOURCE \
 *   -o pXX pXX.cpp -lm
 ******************************************************************************/

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define TRUE 1
#define FALSE 0

#define NMAX 100
#define KMAX 100
#define VUTIL unsigned long int

typedef struct vertex
{
    int valor;
    VUTIL origem;

} Vertex;

/*
0       0
-2      3,4,4
6       2,4,4
4
34
32
30
38      0,1,1
*/

unsigned int k;
int r, q, g;
VUTIL *vbits = NULL; /* Guarda a movimentação de uma unidade em cada uma das direções. */

void compara(Vertex v, int valor, VUTIL origem)
{
    if (v.valor < valor)
    {
        v.valor = valor;
        v.origem = origem;
    }
}

VUTIL verifica_bit(VUTIL n, unsigned int k)
{
    VUTIL aux = 1;

    if (vbits == NULL)
    {
        vbits = (VUTIL*) malloc(sizeof(VUTIL));
        for (unsigned int i = 0; i < sizeof(VUTIL); i++)
            vbits[i] = aux << i;
    }

    return n & vbits[k];
}

int Vizinho( int Q[], int d, int P[])
{
	for (int i = 0; d > 0; i++) {
		P[i] = Q[i] - (d % 2);
		d /= 2;
		if (P[i] < 0) {
			return FALSE;
		}
	}
	return TRUE;
}

void print( int **S, int nS[], int **pos)
{
	unsigned int i, j; // mod;
	for (i = 0; i < k; i++) {
		for (j = 0; j < nS[i]; j++) {
			if (j == pos[i][0] - pos[i][j + 1]) {
				printf( "%c", S[i][j]);
			}
			else {
				printf( "-");
			}
		}
		printf( "\n");
	}
}

int W( int P[], int Q[], char **S);

/* Calcula a nova posição dado o deslocamento. */
int nova_posicao(int pos, VUTIL deslocamento, unsigned int *indexp)
{
    int npos = pos;

    for (int i=0; i<pow(2, k); i++)
        if (verifica_bit(deslocamento, i)) npos -= indexp[i];
    return npos;
}

void alinhamento_rec(char **S, Vertex *Mtz, unsigned int *tams, unsigned int *indexp, unsigned int pos, VUTIL des)
{
    /* Fim da recursão */
    if (pos) {
        int max = (int) pow(2, k);

        /* Calculando todos os deslocamentos possíveis. */
        for( int d=1; d<max; d++) {
            int np = nova_posicao(pos, d, indexp);
            printf("\n%d\n", np);
        }
    }
}





void alinhamento(char **S, Vertex *Mtz, unsigned int *tams, unsigned int *indexp)
{
    alinhamento_rec(S, Mtz, tams, indexp, tams[0]*indexp[0]-1, 0);
}

int main(int argc, char *argv[]) {
	unsigned int *tams, *indexp;
	Vertex *Mtz;
	char **S;

    FILE *input = stdin;
	if (argc > 1) {
		input = fopen( argv[1], "r");
	}

	fscanf( input, "%d %d %d %u", &r, &q, &g, &k);

    if (k > sizeof(VUTIL)) {
        printf("O valor maximo de k e: %d", (int) sizeof(VUTIL));
        return 1;
    }

	S = (char **) malloc( k * sizeof (char *));
	tams = (unsigned int*) malloc( k * sizeof (unsigned int));
	indexp = (unsigned int*) malloc( k * sizeof (unsigned int));

    for ( unsigned int i = 0; i < k; i++ ) {
        S[i] = (char *) malloc( NMAX * sizeof (char));
        fscanf(input, "%s", S[i]);
        tams[i] = strlen(S[i]) + 1;
        indexp[i] = 1;
	}

	for (unsigned int i = k - 1; i > 0; i--) {
		for (unsigned int j = 0; j < i; j++) {
			indexp[j] *= tams[i];
		}
	}
	Mtz = (Vertex *) malloc( tams[0] * indexp[0] * sizeof (Vertex));
	memset( Mtz, 0, tams[0] * indexp[0] * sizeof (Vertex));

    alinhamento(S, Mtz, tams, indexp);

	return 0;
}

int W( int P[], int Q[], char **S) {
	unsigned int i, j;
    int sum = 0;
	for (i = 0; i < k; i++) {
		for (j = i; j < k; j++) {
			if (P[i] == Q[i]) {
				if (P[j] != Q[j]) { /* Q[j] == P[j] + 1 */
					sum += g;
				}
			}
			else if (P[j] == Q[j]) { /* Q[i] == P[i] + 1 */
				sum += g;
			}
			else if (S[i][P[i]] == S[j][P[j]]) { /* encontro */
				sum += r;
			}
			else { /* desencontro */
				sum += q;
			}
		}
	}
	return sum;
}
