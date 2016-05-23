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
#include <limits.h>

#define TRUE 1
#define FALSE 0

#define NMAX 100
#define KMAX 100
#define VUTIL long unsigned int

typedef struct vertex
{
    VUTIL destino;
    VUTIL restricao;
} Vertex;

unsigned int k;
int r, q, g;
VUTIL *vbits = NULL; /* Guarda a movimentação de uma unidade em cada uma das direções. */
int P2k;

int Vizinho(int pos, VUTIL deslocamento, unsigned int *indexp, Vertex *Mtz);
int W( unsigned int P, unsigned int Q, char **S, unsigned int *indexp);

int alinhamento(char **S, Vertex *Mtz, unsigned int *tams, unsigned int *indexp);
void print( char **S, Vertex *Mtz, unsigned int *indexp, unsigned int *tams, char **resp);

VUTIL posicao(unsigned int *pos, unsigned int *indexp);
int inclui(unsigned int *pos, unsigned int *tams, unsigned int pzero);
void zeros(unsigned int *indexp, unsigned int *tams, Vertex *Mtz);

int main(int argc, char *argv[]) {
	unsigned int *tams, *indexp;
	Vertex *Mtz;
	char **S, **resp, c='a';

    FILE *input = stdin;
	if (argc > 1) {
		input = fopen( argv[1], "r");
	}

	fscanf( input, "%d %d %d %u", &r, &q, &g, &k);

    if (k > sizeof(VUTIL)) {
        printf("O valor maximo de k e: %d", (int) sizeof(VUTIL));
        return 1;
    }

    P2k = (int) pow(2, k);

	S = (char **) malloc( k * sizeof (char *));
	resp = (char **) malloc( k * sizeof (char *));
	tams = (unsigned int*) malloc( k * sizeof (unsigned int));
	indexp = (unsigned int*) malloc( k * sizeof (unsigned int));

    for ( unsigned int i = 0; i < k; i++ ) {
        S[i] = (char *) malloc( NMAX * sizeof (char));
        resp[i] = (char *) malloc( NMAX * sizeof (char));
        resp[i][0] = c++;
        resp[i][1] = ':';
        resp[i][2] = ' ';
        fscanf(input, "%s", S[i]);
        tams[i] = strlen(S[i]) + 1;
        indexp[i] = 1;
	}

    fclose(input);

	for (unsigned int i = k - 1; i > 0; i--) {
		for (unsigned int j = 0; j < i; j++) {
			indexp[j] *= tams[i];
		}
	}
	Mtz = (Vertex *) malloc( tams[0] * indexp[0] * sizeof (Vertex));
	memset( Mtz, 0, tams[0] * indexp[0] * sizeof (Vertex));

    /* Preprocessando restrições. */
    zeros(indexp, tams, Mtz);

    /* Procurado melhor alinhamento. */
    printf("\npontos: %d\n", alinhamento(S, Mtz, tams, indexp));
    print(S, Mtz, indexp, tams, resp);

    for ( unsigned int i = 0; i < k; i++ ) {
        free(S[i]);
        free(resp[i]);
    }
    free(resp);
    free(S);
    free(tams);
    free(indexp);

    return 0;
}

/* Calcula a nova posição dado o deslocamento. */
int Vizinho(int pos, VUTIL deslocamento, unsigned int *indexp, Vertex *Mtz)
{
    if (Mtz[pos].restricao & deslocamento) return -1;
    int npos = pos;

    for (int i=0; i<P2k; i++)
        if ( deslocamento & (1<<i) ) npos -= indexp[i];
    return npos;
}

int volta(int pos, VUTIL deslocamento, unsigned int *indexp)
{
    int npos = pos;

    for (int i=0; i<P2k; i++)
        if ( deslocamento & (1<<i) ) npos += indexp[i];
    return npos;
}

void pos2coord(unsigned int coord[], unsigned int pos, unsigned int *indexp) {
    memset(coord, 0, sizeof(coord[0]) * (k));
    for (int i=0; i<k; i++) {
        coord[i] = pos/indexp[i];
        pos %= indexp[i];
    }
}

int W( unsigned int P, unsigned int Q, char **S, unsigned int *indexp) {
    unsigned int i, j;
    unsigned int Pp[sizeof(VUTIL)], Qq[sizeof(VUTIL)];
    unsigned int sum = 0;

    pos2coord(Pp, P, indexp);
    pos2coord(Qq, Q, indexp);

    for (i = 0; i < k; i++) {
		for (j = i; j < k; j++) {
			if (Pp[i] == Qq[i]) {
				if (Pp[j] != Qq[j]) {
					sum += g;
				}
			}
			else if (Pp[j] == Qq[j]) {
				sum += g;
			}
			else if (S[i][Pp[i]] == S[j][Pp[j]]) {
				sum += r;
			}
			else {
				sum += q;
			}
		}
	}
	return sum;
}

int alinhamento_rec(char **S, Vertex *Mtz, unsigned int *tams, unsigned int *indexp, unsigned int pos, VUTIL des)
{
    int pant = volta(pos, des, indexp), /* Ponto anterior. */
        vw,                             /* Valor de retorno das recursões. */
        valor = INT_MIN,                /* Maior valor das recursões. */
        watu = W(pos, pant, S, indexp); /* W calculado com o ponto anterior. */
    VUTIL destino = 0;
    /* Fim da recursão */

    if (pos) {
        int max = P2k;

        /* Calculando todos os deslocamentos possíveis. */
        for( int d=1; d<max; d++) {
            int np = Vizinho(pos, d, indexp, Mtz);
            if (np>=0) {
                vw = alinhamento_rec(S, Mtz, tams, indexp, np, d);
                if (valor < vw) {
                    destino = d;
                    valor = vw;
                }
            }
        }
    }
    else {
        valor = 0;
    }
    Mtz[pos].destino = destino;
    return watu + valor;
}

int alinhamento(char **S, Vertex *Mtz, unsigned int *tams, unsigned int *indexp)
{
    return alinhamento_rec(S, Mtz, tams, indexp, tams[0]*indexp[0]-1, 0);
}

VUTIL posicao(unsigned int *pos, unsigned int *indexp) {
    VUTIL p = 0;
    for (unsigned int i=0; i<k; i++) p += pos[i]*indexp[i];
    return p;
}

int inclui(unsigned int *pos, unsigned int *tams, unsigned int pzero) {
    for (unsigned int i=0; i<k; i++) {
        if (i==pzero) {
            pos[i] = 0;
        }
        else if (pos[i]<tams[i]-1) {
            pos[i]++;
            return 1;
        }
        else {
            pos[i] = 0;
        }
    }
    return 0;
}

/* Inicializando restrições de movimento. */
void zeros(unsigned int *indexp, unsigned int *tams, Vertex *Mtz) {
    unsigned int *aux, x, y;
    aux = (unsigned int *) malloc(k*sizeof(int));
    VUTIL rest = 1;

    /* Restringindo o ponto zero em cada dimensão. */
    for (x=0; x<k; x++) {
        for (y=0; y<x; y++)
            aux[y] = 0;
        Mtz[posicao(aux,indexp)].restricao += rest;
        while (inclui(aux, tams, x))
            Mtz[posicao(aux,indexp)].restricao += rest;
        rest *= 2;
    }
    free(aux);
}

void print_rec( char **S, Vertex *Mtz, unsigned int *indexp, unsigned int *tams, unsigned int p, char **resp, int nums[]) {
    if (p) {
        print_rec(S, Mtz, indexp, tams, Vizinho(p, Mtz[p].destino, indexp, Mtz), resp, nums);
        for (int i=0; i<k; i++) {
            if ( Mtz[p].destino & (1<<i) ) {
                resp[i][3+2*nums[k]] = S[i][nums[i]];
                nums[i]++;
            }
            else resp[i][3+2*nums[k]] = '-';
            resp[i][4+2*nums[k]] = ' ';
        }
        nums[k]++;
    }
}

void print( char **S, Vertex *Mtz, unsigned int *indexp, unsigned int *tams, char **resp)
{
	int nums[sizeof(VUTIL)+1];
	unsigned int max = tams[0]*indexp[0]-1;

    memset(nums, 0, sizeof(nums[0]) * (k+1));
    print_rec(S, Mtz, indexp, tams, max, resp, nums);
    for (int i=0; i<k; i++) {
        resp[i][4+2*nums[k]] = '\0';
        puts(resp[i]);
    }
}
