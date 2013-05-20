/* MAC0438 -- IME/USP
 * Prof. Daniel Batista (batista@ime.usp.br)
 * 9/4/2013
 *
 * baseado no código disponível em
 * https://en.wikipedia.org/wiki/Pthread
 * 
 * Código que ilustra uma das várias
 * soluções para o problema da seção crítica. Cada thread deste código
 * incrementa uma variável compartilhada 100000 vezes. Existem 50
 * threads e portanto o valor final de a, que começa com 0, tem que
 * ser 5000000. Como a solução utilizando semáforos está implementada,
 * o valor final de a sempre será 5000000.
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <semaphore.h>
 
#define NUM_THREADS     50

/* a é minha variável compartilhada pelas threads */
int a;

/* mutex é o semáforo que vou usar para controlar o acesso à seção
 * crítica */
sem_t mutex;

void *TaskCode(void *argument)
{
   int tid;
   int i;

   tid = *((int *) argument);

      sem_wait(&mutex); /* sem_wait = P */
   for (i=0;i<100000;i++) {
      /* Início da seção crítica */
      a++;
      /* Fim da seção crítica */
   }

      sem_post(&mutex); /* sem_post = V */
   fprintf(stderr,"[Olah, eu sou a thread %d e a=%d!]\n", tid,a);

   return NULL;
}
 
int main(void)
{
   pthread_t threads[NUM_THREADS];
   int thread_args[NUM_THREADS];
   int rc,i;

   a=0;
   /* Sempre é bom ter certeza que não existe um mutex já criado com
    * mesmo nome por outro processo no passado */
   sem_unlink("mutex");
   /* Inicialização do semáforo */
   sem_init(&mutex,0,1);
 
   /* criando todas as threads */
   for (i=0; i<NUM_THREADS; ++i) {
      thread_args[i] = i;
      rc = pthread_create(&threads[i], NULL, TaskCode, (void *) &thread_args[i]);
      assert(0==rc); /* se der algum erro na criacao das threads, saio do programa */
   }
 
   /* esperando todas as threads terminarem */
   for (i=0; i<NUM_THREADS; ++i) {
      rc = pthread_join(threads[i], NULL);
      assert(0 == rc);
   }

   /* Para não deixar o mutex criado (pode dar problema se rodar o
    * código de novo) */
   sem_destroy(&mutex); 

   fprintf(stderr,"[Valor final de a=%d]\n",a);
   
   return(0);
}
