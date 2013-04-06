#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h> /* Para trabalhar com threads */
#include <string.h>
#include <unistd.h>

#define NUM_THREADS 5

void *funcao(void *argumentos)
{
	int t = rand()%10;

	sleep(t);
	printf("\nOLA [%d]\n", t);
	return (NULL);
}

int main ()
{
	pthread_t threads[NUM_THREADS];
	int i;

	for(i=0;i < NUM_THREADS;i++)
		pthread_create(&threads[i], NULL, funcao, NULL);
	
	printf("Thread principal a esperar a terminação das threads criadas \n");
	for(i=0;i < NUM_THREADS;i++)
		pthread_join(threads[i],NULL); /* Esperara a junção das threads */
	
	return 0; /* O programa chegará aqui. */
}