#include "fila.h"

/* Inicia uma nova fila. */
fila *novaFila(void) {
	fila *tmp = (fila*) malloc( sizeof(fila) );

	if ( tmp != NULL ) {
		tmp->cabeca = NULL;
		tmp->fim = NULL;
		tmp->n = 0;
	}

	return tmp;
}

/* Insere uma nova informação no fim da fila. */
int insere( fila *f, int info ) {
	if ( f->n == 0 ) {
		f->fim =  (nof*) malloc(sizeof(nof));
		f->cabeca = f->fim;
		f->fim->prox = NULL;
		f->fim->info = info;
		f->n = 1;

		return 1;
	}

	f->fim->prox =  (nof*) malloc(sizeof(nof));

	if ( f->fim->prox != NULL ) {
		f->fim = f->fim->prox;
		f->fim->info = info;
		f->fim->prox = NULL;
		f->n++;
		return 1;
	}
	return 0;
}

/* Busca o próximo da fila. */
int proximo(fila *f) {
	int info = f->cabeca->info;
	nof *tmp = f->cabeca;

	if ( f->cabeca == NULL )
		return -1;

	f->cabeca = f->cabeca->prox;
	f->n--;
	free(tmp);

	return info;
}

/* Libera a fila para a memória. */
int fechou(fila *f) {
	nof *atu;
	nof *prox;

	if ( f == NULL )
		return 0;

	atu = f->cabeca;

	while ( atu != NULL ) {
		prox = atu->prox;
		free(atu);
		atu = prox;
	}
	free(f);

	return 1;
}

/* Retorna o tamanho da fila. */
int tamanho(fila *f) {
	if ( f==NULL )
		return -1;
	return f->n;
}
