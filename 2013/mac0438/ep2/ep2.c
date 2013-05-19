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
long int N0;                                 /* Número do maior termo no momento da sincronização.                                   */
long int m4;                                 /* Variável que acumula parte do cálculo que contém multiplos de 4.                     */
long int m10;                                /* Variável que acumula parte do cálculo que contém multiplos de 10.                    */
long int p2;                                 /* Variável que acumula parte do cálculo que contém potências de 2.                     */
long int n;

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

long double bellard( ) {
	long double termo = 0,
					buffm4,
					buffm10,
					buffp2;
	long int buffN0, buffn;

	/* Efetua a leitura atomicamente */
	sem_wait( &sem_writeread);
	buffm10 = m10;
	m10 *= 10;
	buffm4 = m4;
	m4 *= 4;
	buffp2 = p2;
	p2 *= 1024;
	buffN0 = ++N0;
	buffn = ++n;
	sem_post( &sem_writeread);
	termo += -32. / (buffm4+1 + 4*(buffn-buffN0));
	termo += -1.  / (buffm4+3 + 4*(buffn-buffN0));
	termo += 256. / (buffm10+1 + 10*(buffn-buffN0));
	termo += -64. / (buffm10+3 + 10*(buffn-buffN0));
	termo += -4.  / (buffm10+5 + 10*(buffn-buffN0));
	termo += -4.  / (buffm10+7 + 10*(buffn-buffN0));
	termo += 1.   / (buffm10+9 + 10*(buffn-buffN0));
	termo /= buffp2;
	sem_wait( &sem_writeread);
	(buffn % 2) ? (pi -= termo) : (pi += termo);
	printf( "thread: %d - it: %ld\tpi=%.20Lf\n", param, buffn, pi);
	sem_post( &sem_writeread);

	return (n % 2) ? -termo : termo;
}

void *calculaTermo( void *t) {
	int p = *(int *) t;
	long double termthread;

	/* Enquanto a diferenca entre duas iteracoes consecutivas for maior que f
	 * (parametro de entrada) continua.
	 ***********************************************************************/
	do {
		termthread = bellard( );
	} while (termthread > precisao && termthread > LDBL_EPSILON);
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
printf( "\n\tcores=%d\n\n", numCPU);
	/* Inicializando variaveis globais.
	 *********************************/
	param = 0;
	pi = 0;
	N0 = 0;
	n = 0;
	m4 = 0;
	m10 = 0;
	p2 = 64;
	precisao = LDBL_EPSILON; /* Posteriormente sera parametro de entrada */

	/*
	for( i=0; i<10; i++){
		mem = bellard(i);
		printf("%.100Lf\n", mem);
		pi += mem;

		m4 += 4;
		m10 += 10;
		p2 *= 1024;
	}
	printf("\n\n%.100Lf\n", pi);

	return 0;
	*/

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
	sem_init( &sem_writeread, 0, 1);
	for (t = 0; t < numCPU; ++t)
		pthread_create( &threads[t], NULL, calculaTermo, (void *) t);
	for (t = 0; t < numCPU; ++t)
		pthread_join( threads[t], NULL);
	sem_destroy( &sem_writeread);
	switch(param){
		case 1:
			/* DEBUG */
			break;
		case 2:
			/* SEQUENCIAL */
			termos = (long double*) mallocX(sizeof(long double));
			n = 0;
			do{
				termos[0] = bellard(n);
				pi += termos[0];
				printf("%.20Lf\t%.20Lf\n", pi, termos[0]);
				n++;
				N0++;
				m4 += 4;
				m10 += 10;
				p2 *= 1024;

				if(n==10) break;
			} while( fabs(termos[0])>precisao );

			printf(
				"\nOs resultados obtidos de maneira sequencial foram:\n"
				"pi: %.20Lf\n"
				"termos: %d\n", pi, n
			);
			break;
		default:
			/* NORMAL */
			break;
	}

	return 0;
}