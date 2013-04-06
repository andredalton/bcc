#include "defines.h"

int timeRand(int min, int max);

/* As funções a seguir calculam o tempo gasto em cada etapa para um Atleta a.	*
*  Dependendo da etapa deve ser passado um parametro extra para a função. 		*/
void natacao(Atleta a);
void corrida(Atleta a);

/* Para esta função o tipo de terreno também deve ser passado como parametro de entrada. */
int ciclismo( Atleta a, int terreno);

/* Para esta função é preciso especificar o numero da transicao no segundo parametro. */
int transicao( Atleta a, int t);