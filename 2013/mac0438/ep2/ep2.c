/*******
 * ep2.c
 ******/
/*******************************************************************************
 * Alunos: André Meneghelli Vale,      Núm. USP: 4898948
 *         Marcello Souza de Oliveira, Núm. USP: 6432692
 * Curso: Bacharelado em Ciências da Computação
 * EP 02 -- Calculo do valor aproximado de PI.
 * MAC0438 -- 17/04/2013 -- IME/USP, -- Prof. Daniel Macêdo Batista
 * Compilador: gcc linux 4.6.3
 * Editor: Sublime Text 2;
 * Sistema Operacional: Linux
 ******************************************************************************/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <unistd.h>
#include <float.h>
#include <limits.h>

#define MAX_TERMOS 1000

  /****************************************/
 /* Inicialização das variáveis globais. */
/****************************************/

short int param;                                      /* Variável que guarda qual dos modos de cálculo será utilizado.                        */
long int impreciso;                                   /* Variável usada para indicar que a precisão foi ou não atingida.                      */
long int iteracoes;                                   /* Número de vezes que as threads se encontraram na barreira.                           */

long double *termos;                                  /* Vetor para guardar o resultado das parcelas de cada termo do algoritmo de bellard.   */
long double pi;                              /* Variável para guardar o valor de pi para a máxima precisão da linguagem.             */
long double precisao;                        /* Pecisão do cálculo.                                                                  */
long double p2;                              /* Variável que acumula parte do cálculo que contém potências de 2.                     */
long double m4;                              /* Variável que acumula parte do cálculo que contém multiplos de 4.                     */
long double m10;                             /* Variável que acumula parte do cálculo que contém multiplos de 10.                    */
unsigned long int n;                                  /* Número do termo atual a ser calculado.                                               */

sem_t sem_writeread;                                  /* Semaforo para proteger a leitura e a escrita dos termos précalculados.               */


  /*************************************************/
 /* Termino da declaração das variáveis globais.  */
/*************************************************/


/*pthread_mutex_t meu_mutex = PTHREAD_MUTEX_INITIALIZER;*/

void *mallocX (unsigned int nbytes) {
   void *ptr;
   ptr = malloc (nbytes);
   if (ptr == NULL) {
      printf ("Socorro! malloc devolveu NULL! (%d)\n", nbytes);
      exit (EXIT_FAILURE);
   }
   return ptr;
}

long double bellard(
	long int ln,
	long double lm4,
	long double lm10,
	long double lp2
) {
	long double
		termo = 0;

	termo += (long double) -32 / (lm4+1);
	termo += (long double)  -1 / (lm4+3);
	termo += (long double) 256 / (lm10+1);
	termo += (long double) -64 / (lm10+3);
	termo += (long double)  -4 / (lm10+5);
	termo += (long double)  -4 / (lm10+7);
	termo += (long double)   1 / (lm10+9);
	termo /= lp2;

	return (ln%2) ? -termo : termo;
}

/* Calcula o enésimo termo acumulando os resultados diretamente
 * em pi e sem a necessidade de uso de barreuras.
 *********************************************/
void *calculaTermo_nl( void *t) {
	int p = *(int *) t;
	long int ln;
	long double lm4, lm10, lp2;
	long double termo = 0;

	/* Continua calculando enquanto a precisão não tenha sido atingida anteriormente
	 * por nenhuma thread de um termo menor que a atual.
	 ***********************************************************************/
	do {
		sem_wait( &sem_writeread);
		ln = n++;
		lm4 = m4; m4 += 4;
		lm10 = m10; m10 += 10;
		lp2 = p2; p2 *= 1024;
		sem_post( &sem_writeread);

		if( impreciso!=-1 && ln>impreciso)
			break;

		termo = bellard( ln, lm4, lm10, lp2);

		pi += termo;
		printf( "thread: %d - it: %ld\tpi: %.20Lf\ttermo: %g\n", p, ln, pi, (double)termo );
	} while ( fabs(termo)>precisao && impreciso==-1 );
	
	if(impreciso==-1)
		impreciso = n;

	return NULL;
}

/* Calcula o enésimo termo acumulando os resultados diretamente
 * em pi e sem a necessidade de uso de barreuras.
 *********************************************/
void *calculaTermo( void *t) {
	calculaTermo_nl(t);

	/*
	int p = *(int *) t;
	long int ln;
	long double lm4, lm10, lp2;
	long double termo = 0;
	*/

	/* Continua calculando enquanto a precisão não tenha sido atingida anteriormente
	 * por nenhuma thread de um termo menor que a atual.
	 ***********************************************************************/
	/*
	do {
		sem_wait( &sem_writeread);
		ln = n++;
		lm4 = m4; m4 += 4;
		lm10 = m10; m10 += 10;
		lp2 = p2; p2 *= 1024;
		sem_post( &sem_writeread);

		if( impreciso!=-1 && ln>impreciso)
			break;

		termo = bellard( ln, lm4, lm10, lp2);

		pi += termo;
		printf( "thread: %d - it: %ld\tpi: %.20Lf\ttermo: %g\n", p, ln, pi, (double)termo );
	} while ( fabs(termo)>precisao && impreciso==-1 );
	
	if(impreciso==-1)
		impreciso = n;
	*/
	return NULL;
}

/* Função principal.
 *******************/
int main(int argc, char *argv[]){
	int
		i, t,
		n,
		numCPU = sysconf( _SC_NPROCESSORS_ONLN );
	pthread_t *threads = (pthread_t *) mallocX( numCPU * sizeof (pthread_t));
	printf( "Utilizando %d nucleos.\n\n", numCPU);

	/* Inicializando variaveis globais.
	 *********************************/
	param = 0;
	iteracoes = 0;
	impreciso = -1;
	pi = 0;
	n = 0;
	m4 = 0;
	m10 = 0;
	p2 = 64;

	/* Lendo os parametros de entrada.
	 *********************************/
	if(argc<2) {
		printf(
			"Modo de uso:\n"
			"%s [DEBUG] [SEQUENCIAL] [UNLIMITED] <f>\n"
			"\tDEBUG: para rodar em modo de depuracao;\n"
			"\tSEQUENCIAL: rodar sem o uso de threads;\n"
			"\tUNLIMITED: rodar sem ter que parar na barreira;\n"
			"\t<f>: precisao do calculo.\n", argv[0]
		);
	}
	else {
		for(i=1; i<argc; i++) {
			if( strcmp(argv[i], "DEBUG")==0 )
				param = 1;
			else if (strcmp(argv[i], "SEQUENCIAL")==0 )
				param = 2;
			else if (strcmp(argv[i], "UNLIMITED")==0 )
				param = 3;
			sscanf(argv[i], "%Lf", &precisao);
		}
	}

	/*
	pthread_t threads[NUM_THREADS];
	for (t=0; t<NUM_THREADS; t++)
		pthread_create(&threads[t], NULL, AtualizaSaldo, (void *)t);
	*/
	/*
    for (i = 0; i < numCPU; ++i) {
        pthread_attr_init( thread_attrs + i);
        pthread_attr_setdetachstate( thread_attrs + i, PTHREAD_CREATE_JOINABLE);
        pthread_create( threads + i, thread_attrs + i, worker_function, (void *) i);
    }
	for (t = 0; t < numCPU; t++)
		pthread_join( threads[t], NULL);
	*/

	switch(param){
		case 2:
			/* SEQUENCIAL */
			termos = (long double*) mallocX(sizeof(long double));
			n = 0;
			do{
				*termos = bellard( n, m4, m10, p2 );
				pi += *termos;
				printf("%.40Lf\t[%g]\n", pi, (double) *termos);
				n++;
				m4 += 4;
				m10 += 10;
				p2 *= 1024;
			} while( fabs(*termos)>precisao && p2 != 0 );

			printf("\nO resultado de pi obtido com %d termos calculados de maneira sequencial foi:\n%.40Lf\n", n, pi);
			break;
		case 3:
			/* UNLIMITED */
			sem_init( &sem_writeread, 0, 1);
			for (t = 0; t < numCPU; ++t)
				pthread_create( &threads[t], NULL, calculaTermo_nl, (void *) &t);
			for (t = 0; t < numCPU; ++t)
				pthread_join( threads[t], NULL);
			sem_destroy( &sem_writeread);
			printf("\nO resultado de pi obtido usando %d processos foi:\n%.40Lf\n", numCPU, pi);
			break;
		default:
			/* DEBUG */
			/* NORMAL */
			termos = (long double*) mallocX( MAX_TERMOS*sizeof(long double) );

			sem_init( &sem_writeread, 0, 1);
			for (t = 0; t < numCPU; ++t)
				pthread_create( &threads[t], NULL, calculaTermo, (void *) &t);
			for (t = 0; t < numCPU; ++t)
				pthread_join( threads[t], NULL);
			sem_destroy( &sem_writeread);
			printf("\nO resultado de pi obtido usando %d processos foi:\n%.40Lf\n", numCPU, pi);
			break;
	}

	return 0;
}