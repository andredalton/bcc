/*************************************************************
*
*	Biblioteca de lista ligada.
*
*
*	@author: Vitor Lima & André Dalton
**************************************************************/


#include"lista_ligada.h"
#include<stdlib.h>

struct no{
	int pid;
	NO prox;
};


NO cria_lista (int pid){
	NO n = (NO) malloc (sizeof (struct no));
	n->pid = pid;
	n->prox = NULL;
	return n;
}

/************************************************
*	Destrói todos os elementos da lista passada.
************************************************/
void destroi_lista (NO* lista){
	if (*lista){
		destroi_lista (&((*lista)->prox));
		free (*lista);
		*lista = NULL;
	}
}

/**********************************************************
*	Procura por <valor> em <lista>.
*	Devolve o primeiro NO da <lista> que tiver o <valor>,
* ou NULL, caso não encontre.
***********************************************************/
NO esta_na_lista (NO lista, int valor){
	while (lista){
		if (valor == lista->pid)
			return lista;
		lista = lista->prox;
	}
	return NULL;
}


NO pega_prox (NO lista){
	if (lista)
		return lista->prox;
	return lista;
}

int pega_valor (NO lista){
	if (lista)
		return lista->pid;
	return INVALID_PID;
}

/******************************************
*	Insere um novo elemento no começo da
* lista ligada passada.
************************************************/
void insere (NO* lista, int pid){
	NO novo = cria_lista (pid);
	NO p = *lista;
	if (*lista){ /* se lista não está vazia */
		novo->prox = *lista;
		*lista = novo;
	}else{
		*lista = novo;
	}

}

/****************************************************
*		Procura na lista pelo nó cujo valor seja
* igual a x.
*		Se encontrar, remove-o. Senão, deixa a lista como está.
*****************************************************/
void remove_no (NO* lista, int x){
	NO ant = NULL;
	NO p = *lista;

	while (p && p->pid != x){ /* lista não nula */
		ant = p;
		p = p->prox;
	}
	if (p){ /* se encontrou o nó que contém o valor x */
		if (ant == NULL){ /* se o nó a ser removido é o primeiro */
			*lista = p->prox;
		}else {
			ant->prox = p->prox;
		}
		free (p);
	}
}



