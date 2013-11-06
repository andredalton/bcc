/*************************************************************
*
*	Biblioteca de lista ligada.
*
*
*	@author: Vitor Lima & André Dalton
**************************************************************/

#define INVALID_PID -1234567

typedef struct no* NO;

NO cria_lista (int pid);

NO pega_prox (NO lista);

int pega_valor (NO lista);

/******************************************
*	Insere um novo elemento no começo da
* lista ligada passada.
************************************************/
void insere (NO* lista, int pid);

/************************************************
*	Destrói todos os elementos da lista passada.
************************************************/
void destroi_lista (NO lista);
