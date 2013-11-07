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

/************************************************
*	Destrói todos os elementos da lista passada.
************************************************/
void destroi_lista (NO* lista);

/**********************************************************
*	Procura por <valor> em <lista>.
*	Devolve o primeiro NO da <lista> que tiver o <valor>,
* ou NULL, caso não encontre.
***********************************************************/
NO esta_na_lista (NO lista, int valor);

NO pega_prox (NO lista);

int pega_valor (NO lista);

/******************************************
*	Insere um novo elemento no começo da
* lista ligada passada.
************************************************/
void insere (NO* lista, int pid);


/****************************************************
*		Procura na lista pelo nó cujo valor seja
* igual a x.
*		Se encontrar, remove-o. Senão, deixa a lista como está.
*****************************************************/
void remove (NO* lista, int x);
