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

#include "precisao.h"

  /****************************************/
 /* Inicialização das variáveis globais. */
/****************************************/

short int param;                /* Variável que guarda se roda no modo normal, sequencial ou debug.                     */

/*short int N;                    / Diferença entre o termo atual e maior termo no momento de sincronização.             */
/*long double buffer;             / Buffer utilizado para a conversão dos dados fracionários para a estrutura SuperLong. */
/*SuperLong N0;                   / Número do maior termo no momento da sincronização.                                   */
/*SuperLong m4;                   / Variável que acumula parte do cálculo que contém multiplos de 4.                     */
/*SuperLong m10;                  / Variável que acumula parte do cálculo que contém multiplos de 10.                    */
/*SuperLong p2;                   / Variável que acumula parte do cálculo que contém potências de 2.                     */
/*SuperLong pi;                   / Variável para guardar o valor de pi em longa precisão.                               */
/*SuperLong precisao;             / Pecisão do cálculo.                                                                  */

/*
sem_t sem_estrada[CITAM];
*/
  /*************************************************/
 /* Termino da declaração das variáveis globais.  */
/*************************************************/


long double pi;                /* Variável para guardar o valor de pi para a máxima precisão da linguagem.             */
long double precisao;          /* Pecisão do cálculo.                                                                  */
long int N0;                   /* Número do maior termo no momento da sincronização.                                   */
long int m4;                   /* Variável que acumula parte do cálculo que contém multiplos de 4.                     */
long int m10;                  /* Variável que acumula parte do cálculo que contém multiplos de 10.                    */
long int p2;                   /* Variável que acumula parte do cálculo que contém potências de 2.                     */

long double bellard( int n) {
	long double termo = 0;

	termo += -32. / (m4+1);				printf("\t[%.100Lf]\n", termo);
	termo += -1.  / (m4+3);				printf("\t[%.100Lf]\n", termo);
	termo += 256. / (m10+1);			printf("\t[%.100Lf]\n", termo);
	termo += -64. / (m10+3);			printf("\t[%.100Lf]\n", termo);
	termo += -4.  / (m10+5);			printf("\t[%.100Lf]\n", termo);
	termo += -4.  / (m10+7);			printf("\t[%.100Lf]\n", termo);
	termo += 1.   / (m10+9);			printf("\t[%.100Lf]\n", termo);
	termo /= p2;						printf("\t[%.100Lf]\n", termo);

	return (n%2) ? -termo : termo;
}

long double bellardinho( int n, int k) {
	switch (k) {
		case 0: return (n%2) ?   32 /  (m4 + 1 + 4 * (n - N0)) : -32 /  (m4 + 1 + 4 * (n - N0));
		case 1: return (n%2) ?    1 /  (m4 + 3 + 4 * (n - N0)) :  -1 /  (m4 + 3 + 4 * (n - N0));
		case 2: return (n%2) ? -256 / (m10 + 3 + 4 * (n - N0)) : 256 / (m10 + 3 + 4 * (n - N0));
		case 3: return (n%2) ?   64 / (m10 + 3 + 4 * (n - N0)) : -64 / (m10 + 3 + 4 * (n - N0));
		case 4: return (n%2) ?    4 / (m10 + 3 + 4 * (n - N0)) :  -4 / (m10 + 3 + 4 * (n - N0));
		case 5: return (n%2) ?    4 / (m10 + 3 + 4 * (n - N0)) :  -4 / (m10 + 3 + 4 * (n - N0));
		case 6: return (n%2) ?   -1 / (m10 + 3 + 4 * (n - N0)) :   1 / (m10 + 3 + 4 * (n - N0));
	}
	return precisao;
}

/* Função principal.
 *******************/
int main(int argc, char *argv[]){
	int
		i,
		j,
		n,
		numCPU = sysconf( _SC_NPROCESSORS_ONLN );
	double d, s=0, d2;
	long double mem;
	
	/* Inicializando variaveis globais.
	 *********************************/
	param = 0;
	pi = 0;
	N0 = 0;
	m4 = 0;
	m10 = 0;
	p2 = 64;

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

	if(argc<2) {
		printf(
			"Modo de uso:\n"
			"%s DEBUG SEQUENCIAL <f>\n"
			"DEBUG: para rodar em modo de depuracao.\n"
			"SEQUENCIAL: rodar sem o uso de threads.\n"
			"<f>: precisao do calculo.\n", argv[0]
		);
	}
	else {
		/*
		for(i=1; i<argc; i++) {
			if( strcmp(argv[i], "DEBUG")==0 )
				param = 1;
			else if (strcmp(argv[i], "SEQUENCIAL")==0 )
				param = 2;
			readSuperLong(&precisao, argv[i]);
		}
		printf("\n");
		printSuperLong(precisao);
		printf("\n");
		SuperLongSomaInt(precisao, 1);
		printSuperLong(precisao);		
		printf("\n");
		*/
	}

	return 0;
}