#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#define NUM_THREADS 5

pthread_mutex_t meu_mutex = PTHREAD_MUTEX_INITIALIZER;
int saldo = 1000;

void *AtualizaSaldo( void *threadid) {
   pthread_mutex_lock( &meu_mutex);
   saldo = saldo + (int) threadid * 100;
   printf( "Novo saldo = %d\n", saldo);
   pthread_mutex_unlock( &meu_mutex);
}

int main( int argc, char argv[]) {
   pthread_t threads[NUM_THREADS];
   long t;
   for (t = 0; t < NUM_THREADS; t++)
      pthread_create( &threads[t], NULL, AtualizaSaldo, (void *) t);
   for (t = 0; t < NUM_THREADS; t++)
      pthread_join( threads[t], NULL);
   printf( "Saldo final = %d\n", saldo);
}

