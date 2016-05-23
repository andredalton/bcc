/*
 * ep1.c
 *
 *  Created on: Apr 13, 2016
 *      Author: mcello
 */

/*******************************************************************************
 * MAC0327 -- IME/USP
 * Aluno: Marcello Souza de Oliveira, 6432692
 *        André Meneghelli Vale, 4898948
 * Curso: Bacharelado em Ciencias da Computacao
 * Compilador: g++ linux 4.9.2
 * Opcoes compilacao: -Wall -ansi -pedantic -O2 -U_FORTIFY_SOURCE -lm
 * Compilar:
 *  gcc -Wall -ansi -pedantic -O2 -U_FORTIFY_SOURCE -o ep1 ep1.c -lm
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

unsigned long int dbgprint = 0;

#define TRUE 1
#define FALSE 0

#define NMAX 100
#define KMAX 100
#define VUTIL unsigned long int

typedef struct vertex {
	int valor;
	VUTIL destino;
} Vertex;


int k = 3;
int r = 10, q = -1, g = -3;
int DOISK = 8; /* 2^k */


int alignment( Vertex p[], int indexp[], char **S, int Q[], int **pos, unsigned long int it);

int Vizinho( int Q[], int d, int P[]);

int W( int P[], int Q[], char **S);

void print_pos( char **S, int nS[], int **pos);

void set_pos_forward( int P[], int d, int **pos, int it);

void print_rec( char **S, Vertex *Mtz, unsigned int *indexp, unsigned int *tams, unsigned int p, char resp[][2*NMAX], int nums[]) {
    if (p) {
        print_rec(S, Mtz, indexp, tams, Vizinho(p, Mtz[p].destino, indexp, Mtz), resp, nums);
        for (int i=0; i<k; i++) {
            if (verifica_bit(Mtz[p].destino, i)) {
                resp[i][3+2*nums[k]] = S[i][nums[i]];
                nums[i]++;
            }
            else resp[i][3+2*nums[k]] = '-';
            resp[i][4+2*nums[k]] = ' ';
        }
        nums[k]++;
    }
}

void print( char **S, Vertex *Mtz, unsigned int *indexp, unsigned int *tams)
{
	int nums[sizeof(VUTIL)+1];
	char resp[sizeof(VUTIL)][2*NMAX];
	unsigned int max = tams[0]*indexp[0]-1;

    memset(nums, 0, sizeof(nums[0]) * (k+1));
    print_rec(S, Mtz, indexp, tams, max, resp, nums);
    for (int i=0; i<k; i++) {
        resp[i][4+2*nums[k]] = '\0';
        puts(resp[i]);
    }
}

int main(int argc, char *argv[]) {
	char **S;
	int **pos;
	int *nS, /* length of sequences */
	    *indexp; /* powers in lengths to manipulate k-array as 1-array */
	int i, j, np, ret;
	Vertex *p; /* k-dimensional matrix */

	FILE *input = stdin;
	if (argc > 1) {
		input = fopen( argv[1], "r");
	}

	ret = fscanf( input, "%d %d %d %d", &r, &q, &g, &k);
	if (!ret) {
		return 1;
	}
	DOISK = pow( 2, k);

	S = (char **) malloc( k * sizeof (char *));
	pos = (int **) malloc( k * sizeof (int *));
	nS = (int *) malloc( k * sizeof (int));
	indexp = (int *) malloc( k * sizeof (int));
	if (S == NULL) {
		return 1;
	}

	for (i = 0; i < k; i++) {
		S[i] = (char *) malloc( NMAX * sizeof (char));
		pos[i] = (int *) malloc( NMAX * sizeof (int));
		memset( pos[i], 0, NMAX * sizeof (int));
		/* fgets( S[i], sizeof (S[i]), stdin); */
		ret = fscanf( input, " %s", S[i]);
		if (!ret) {
			return 1;
		}
		nS[i] = strlen( S[i]);
		indexp[i] = 1;
	}
	for (i = k - 1; i > 0; i--) {
		for (j = 0; j < i; j++) {
			indexp[j] *= (nS[i] + 1);
		}
	}
	np = (nS[0] + 1) * indexp[0];
	p = (Vertex *) malloc( np * sizeof (Vertex));
	memset( p, 0, np * sizeof (Vertex)); /* note: p[nS].destino = 0 */
	/*
	p[0].destino = 0;
	p[0].valor = 0;
	*/

	for (i = 0; i < k; i++) {
		printf( "    %s\n", S[i]);
	}
	printf( ">>> %d\n", alignment( p, indexp, S, nS, pos, 0));
	// print( p, indexp, S, nS, pos);
	print(S, p, indexp, nS );

	if (argc > 1) {
		fclose( input);
	}

	return 0;
}

int alignment( Vertex p[], int indexp[], char **S, int Q[], int **pos, unsigned long int it) {
	int i, j, t;
	int *P, maxV[2] = { 0, 0 }, V[2] = { 0, 0 };

	for (t = 0, i = 0; i < k; i++) {
		if (Q[i] != 0) {
			t++;
		}
	}
	if (t == 0) {
		return 0;
	}

	P = (int *) malloc( k * sizeof (int));
	for (i = 1; i < DOISK; i++) {
		if (Vizinho( Q, i, P) == TRUE) {
			for (j = 0, t = 0; t < k - 1; t++) {
				j += P[t] * indexp[t];
			}
			j += P[k - 1];

			if (p[j].valor != 0) {
				V[0] = W( P, Q, S) + p[j].valor;
			}
			else {
				V[0] = W( P, Q, S) + alignment( p, indexp, S, P, pos, it + 1);
			}
			V[1] = i;

			if (V[0] > maxV[0]) {
				maxV[0] = V[0];
				maxV[1] = V[1];
			}
		}
	}

	/*
	 * p[Q].valor = max{...}
	 */
	for (j = 0, i = 0; i < k - 1; i++) {
		j += Q[i] * indexp[i];
	}
	j += Q[k - 1];
	p[j].valor = maxV[0];
printf( " ----- Q=%d --> %d\n", j, p[j].valor);

	/*
	 * p[P].destino = Q
	 */
	Vizinho( Q, maxV[1], P);
	for (j = 0, i = 0; i < k - 1; i++) {
		j += P[i] * indexp[i];
	}
	j += P[k - 1];
	p[j].destino = maxV[1];
printf( " ----- P=%d --> %lu | %d\n", j, p[j].destino, p[j].valor);

	free( P);

	return maxV[0];
}

int Vizinho( int Q[], int d, int P[]) {
	int i;
	for (i = 0; i < k; i++) {
		P[i] = Q[i] - (d % 2); /* note: d representa um binário invertido */
		d /= 2;
		if (P[i] < 0) {
			return FALSE;
		}
	}
	return TRUE;
}

void set_pos_forward( int P[], int d, int **pos, int it) {
	int i, mod2;

	for (i = 0; i < k; i++) {
		mod2 = d % 2;

		if (mod2 == 0) {
			pos[i][it] = it;
		}
		else {
			pos[i][it] = it + 1;
		}

		P[i] += mod2;
		d /= 2;
	}
}

/*
void print( Vertex p[], int indexp[], char **S, int nS[], int **pos) {
	int i, j, t, d;
	int *P;

	P = (int *) malloc( k * sizeof (int));
	memset( P, 0, k * sizeof (int));

	d = p[0].destino;
	for (i = 0; d != 0; i++) {
		set_pos_forward( P, d, pos, i);
		for (j = 0, t = 0; t < k - 1; t++) {
			j += P[t] * indexp[t];
		}
		j += P[k - 1];
		d = p[j].destino;
printf( " ----- %lu\n", p[j].destino);
	}

	printf( "\n");
	for (i = 0; i < k; i++) {
		for (j = 0; j < nS[i]; j++) {
			printf( "%d", pos[i][j]);
		}
		printf( "\n");
	}
	printf( "\n");
	for (i = 0; i < (nS[0] + 1) * indexp[0]; i++) {
		printf( "%lu", p[i].destino);
	}
	printf( "\n");

	printf( "P: ");
	for (i = 0; i < k; i++) {
		printf( "%d", P[i]);
	}
	printf( "\n");

	print_pos( S, nS, pos);

/*test
	pos[0][0] = 3; pos[0][1] = 5; pos[0][2] = 6;
	pos[1][0] = 0; pos[1][1] = 1; pos[1][2] = 2; pos[1][3] = 3;
	pos[2][0] = 0; pos[2][1] = 3; pos[2][2] = 4; pos[2][3] = 5;

	printf( "\nNOVA POS:\n");
	print_pos( S, nS, pos);
*/
}
*/
void print_pos( char **S, int nS[], int **pos) {
	int i, j, len = 0, il;
	for (i = 0; i < k; i++) {
		if (pos[i][nS[i] - 1] > len) {
			len = pos[i][nS[i] - 1];
		}
	}
	for (i = 0; i < k; i++) {
		for (il = 0, j = 0; j < nS[i]; j++, il++) {
			if (il == pos[i][j]) {
				printf( "%c", S[i][j]);
			}
			else {
				for (; il < pos[i][j]; il++) {
					printf( "-");
				}
				printf( "%c", S[i][j]);
			}
		}
		for (; il <= len; il++) {
			printf( "-");
		}
		printf( "\n");
	}
}

int W( int P[], int Q[], char **S) {
	int i, j,
	    sum = 0;
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
