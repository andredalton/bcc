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

short int N;                    /* Diferença entre o termo atual e maior termo no momento de sincronização.             */
long double buffer;             /* Buffer utilizado para a conversão dos dados fracionários para a estrutura SuperLong. */
SuperLong N0;                   /* Número do maior termo no momento da sincronização.                                   */
SuperLong m4;                   /* Variável que acumula parte do cálculo que contém multiplos de 4.                     */
SuperLong m10;                  /* Variável que acumula parte do cálculo que contém multiplos de 10.                    */
SuperLong p2;                   /* Variável que acumula parte do cálculo que contém potências de 2.                     */
SuperLong pi;                   /* Variável para guardar o valor de pi em longa precisão.                               */
SuperLong precisao;             /* Pecisão do cálculo.                                                                  */

/*
sem_t sem_estrada[CITAM];
*/
  /*************************************************/
 /* Termino da declaração das variáveis globais.  */
/*************************************************/

/* Função principal.
 *******************/
int main(int argc, char *argv[]){
	int
		i,
		j,
		n,
		numCPU = sysconf( _SC_NPROCESSORS_ONLN );
	double d, s=0, d2;
	
	/* Inicializando variaveis globais. */
	param = 0;

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
	}

	return 0;
}