/*
 * Implementação assimétrica do jantar dos filósofos.
 *
 * Código da Professora Islene do IC-Unicamp. Com alguns comentários
 * adicionados pelo Prof. Daniel Batista (batista@ime.usp.br) para a disciplina
 * MAC0438 em 09/04/2013. Esse código implementa uma solução parecida
 * com a vista em sala de aula em que nem todos os filósofos pegam os
 * garfos na mesma ordem. Na sala de aula o algoritmo visto fazia o
 * último filósofo pegar os garfos na ordem contrária. Aqui a ordem
 * depende do ID do filósofo. Se for par pega em uma ordem. Se for
 * ímpar pega em outra ordem. Isso é suficiente para resolver o
 * problema do deadlock e é uma solução com paralelismo maior do que a
 * solução com o sem_central.
 *
 * Código copiado de
 * http://www.ic.unicamp.br/~islene/mc514/filosofos.tgz
 * 
 */

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 5
#define N_CICLOS 8
     
sem_t sem_garfo[N];

/* Variáveis extra para mostrar o estado da mesa */
sem_t sem_mesa;
enum pos_garfo {ESQ, MESA, DIR} pos_garfo[N];
char mesa[4*N+1];

/*
 *  garfo esq = phil_id
 *  garfo dir = (phil_id + 1) % N
 */

void print() {
  printf("%s%c%c\n", &mesa[1], mesa[0], mesa[1]);
}

void fica_com_fome(int phil_id) {
  sem_wait(&sem_mesa);
  mesa[4 * phil_id + 3] = 'H';
  print();
  sem_post(&sem_mesa);
  sleep(1);
}

void come(int phil_id) {
  sem_wait(&sem_mesa);
  mesa[4 * phil_id + 3] = 'E';
  print();
  sem_post(&sem_mesa);
  sleep(1);
}

void pega_garfo_esq(int phil_id) {
  sem_wait(&sem_garfo[phil_id]);
  sem_wait(&sem_mesa);
  mesa[4 * phil_id] = ' ';
  mesa[4 * phil_id + 1] = ' ';
  mesa[4 * phil_id + 2] = '|';
  print();
  sem_post(&sem_mesa);
  sleep(1);
}  

void pega_garfo_dir(int phil_id) {
  int dir = (phil_id + 1) % N;
  sem_wait(&sem_garfo[dir]);
  sem_wait(&sem_mesa);
  mesa[4 * dir] = '|';
  mesa[4 * dir + 1] = ' ';
  mesa[4 * dir + 2] = ' ';
  print();
  sem_post(&sem_mesa);
  sleep(1); 
}

void solta_garfos_pensa(int phil_id) {
  int dir = (phil_id + 1) % N;
  sem_wait(&sem_mesa);
  sem_post(&sem_garfo[dir]);
  sem_post(&sem_garfo[phil_id]);
  mesa[4 * phil_id] = ' ';
  mesa[4 * phil_id + 1] = '-';
  mesa[4 * phil_id + 2] = ' ';
  mesa[4 * dir] = ' ';
  mesa[4 * dir + 1] = '-';
  mesa[4 * dir + 2] = ' ';
  mesa[4 * phil_id + 3] = 'T';
  print();
  sem_post(&sem_mesa);
  sleep(1);
}


void* f_phil(void *v) {
  int phil_id = *(int *) v;
  int i;


  for (i = 0; i < N_CICLOS; i++) {
    fica_com_fome(phil_id);

    if (phil_id % 2) { 
      pega_garfo_dir(phil_id);
      pega_garfo_esq(phil_id);
    } else {
          pega_garfo_esq(phil_id);
	  pega_garfo_dir(phil_id);
    }
    come(phil_id);
	
    solta_garfos_pensa(phil_id);
  }
  
  return NULL;
}

int main() {
  pthread_t thr[N];
  int i, phil_id[N];

  sem_init(&sem_mesa, 0, 1);
  for (i = 0; i < N; i++) {
      sem_init(&sem_garfo[i], 0, 1);
      pos_garfo[i] = MESA;
      mesa[4 * i] = ' ';
      mesa[4 * i + 1] = '-';
      mesa[4 * i + 2] = ' ';
      mesa[4 * i + 3] = 'T';
  }
  mesa[4*N] = 0;

  for (i = 0; i < N; i++) {
    phil_id[i] = i;
    pthread_create(&thr[i], NULL, f_phil, (void*) &phil_id[i]);
  }

  for (i = 0; i < N; i++) 
    pthread_join(thr[i], NULL);

  for (i = 0; i < N; i++) 
    sem_destroy(&sem_garfo[i]);
  sem_destroy(&sem_mesa);
    
  return 0;
}
