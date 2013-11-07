#include <stdio.h>
#include <stdlib.h>

typedef struct No {
	int info;
	struct No* prox;
} nof;

typedef struct Fi {
	nof *cabeca;
	nof *fim;
	int n;
} fila;

fila *novaFila();
int entra( fila *f, int info );
int proximo( fila *f );
int fechou( fila *f );
int tamanho( fila *f );
