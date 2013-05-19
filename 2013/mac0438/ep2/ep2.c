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
#include <pthread.h> /* Para trabalhar com threads */
#include <semaphore.h>
#include <string.h>
#include <unistd.h>
#include <float.h>
#include <limits.h>

#define MAX_TERMOS 1000

  /****************************************/
 /* Inicialização das variáveis globais. */
/****************************************/

short int param;                /* Variável que guarda se roda no modo normal, sequencial ou debug.                     */

/*
sem_t sem_estrada[CITAM];
*/
  /*************************************************/
 /* Termino da declaração das variáveis globais.  */
/*************************************************/

long double pi;                              /* Variável para guardar o valor de pi para a máxima precisão da linguagem.             */
long double precisao;                        /* Pecisão do cálculo.                                                                  */
long double *termos;                         /* Vetor para guardar o resultado das parcelas de cada termo do algoritmo de bellard.   */
long double p2;                              /* Variável que acumula parte do cálculo que contém potências de 2.                     */
long double m4;                                 /* Variável que acumula parte do cálculo que contém multiplos de 4.                     */
long double m10;                                /* Variável que acumula parte do cálculo que contém multiplos de 10.                    */
long int n;												/* Número do maior termo no momento da sincronização.

/*pthread_mutex_t meu_mutex = PTHREAD_MUTEX_INITIALIZER;*/
sem_t sem_writeread;

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

	return termo;
}

void *calculaTermo( void *t) {
	int p = *(int *) t;
	long int ln, lm4, lm10, lp2;
	long double termthread;

	/* Enquanto a diferenca entre duas iteracoes consecutivas for maior que f
	 * (parametro de entrada) continua.
	 ***********************************************************************/
	do {
		sem_wait( &sem_writeread);
		ln = n++;
		lm4 = m4; m4 += 4;
		lm10 = m10; m10 += 10;
		lp2 = p2; p2 *= 1024;
		sem_post( &sem_writeread);
		termthread = bellard( ln, lm4, lm10, lp2);
		sem_wait( &sem_writeread);
		(ln % 2) ? (pi -= termthread) : (pi += termthread);
		sem_post( &sem_writeread);
		printf( "thread: %d - it: %ld\ttermo=%.20Lf\tpi=%.20Lf\n", p, ln, termthread, pi);
	} while (fabs( termthread) > precisao && fabs( termthread) > LDBL_EPSILON);
	return NULL;
}

/* Função principal.
 *******************/
int main(int argc, char *argv[]){
	int
		i, j, t,
		n,
		numCPU = sysconf( _SC_NPROCESSORS_ONLN );
	double d, s=0, d2;
	long double mem;
	pthread_t *threads = (pthread_t *) mallocX( numCPU * sizeof (pthread_t));
	printf( "Utilizando %d nucleos.\n\n", numCPU);

	/* Inicializando variaveis globais.
	 *********************************/
	param = 0;
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
			"%s [DEBUG] [SEQUENCIAL] <f>\n"
			"\tDEBUG: para rodar em modo de depuracao;\n"
			"\tSEQUENCIAL: rodar sem o uso de threads;\n"
			"\t<f>: precisao do calculo.\n", argv[0]
		);
	}
	else {
		for(i=1; i<argc; i++) {
			if( strcmp(argv[i], "DEBUG")==0 )
				param = 1;
			else if (strcmp(argv[i], "SEQUENCIAL")==0 )
				param = 2;
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
		case 1:
			/* DEBUG */
			break;
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

			printf(
				"\nOs resultados obtidos de maneira sequencial foram:\n"
				"pi: %.30Lf\n"
				"termos: %d\n", pi, n
			);
			break;
		default:
			/* NORMAL */
			termos = (long double*) mallocX( MAX_TERMOS*sizeof(long double) );

			sem_init( &sem_writeread, 0, 1);
			for (t = 0; t < numCPU; ++t)
				pthread_create( &threads[t], NULL, calculaTermo, (void *) &t);
			for (t = 0; t < numCPU; ++t)
				pthread_join( threads[t], NULL);
			sem_destroy( &sem_writeread);
			break;
	}

	return 0;
}