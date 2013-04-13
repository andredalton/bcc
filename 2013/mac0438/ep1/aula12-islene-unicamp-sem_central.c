/*
 * Implementação com lock central e baixíssimo paralelismo.
 *
 * Código da Professora Islene do IC-Unicamp. Com alguns comentários
 * adicionados pelo Prof. Daniel Batista (batista@ime.usp.br) para a disciplina
 * MAC0438 em 09/04/2013. Esse código implementa uma solução que
 * inclui um semáforo a mais (sem_t sem_central) comum a todos os filósofos que protege
 * todo o código para pegar os garfos (no final das contas esse trecho
 * é uma seção crítica). O problema é que a solução fica com um
 * paralelismo baixo e o desempenho cai com isso. A vantagem é que não
 * acontece deadlock aqui.
 *
 * Código copiado de
 * http://www.ic.unicamp.br/~islene/mc514/filosofos.tgz
 */

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 10
#define N_CICLOS 8
     
sem_t sem_garfo[N];
sem_t sem_central;

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
    
    sem_wait(&sem_central);
    pega_garfo_dir(phil_id);
    pega_garfo_esq(phil_id);

    come(phil_id);
	
    solta_garfos_pensa(phil_id);
    sem_post(&sem_central);
  }
  
  return NULL;
}

int main() {
  pthread_t thr[N];
  int i, phil_id[N];

  sem_init(&sem_central, 0, 1);
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
  sem_destroy(&sem_central);
  return 0;
}
