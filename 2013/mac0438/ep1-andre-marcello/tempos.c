/**********
 * tempos.c
 *********/
/*******************************************************************************
 * Alunos: André Meneghelli Vale,      Núm. USP: 4898948
 *         Marcello Souza de Oliveira, Núm. USP: 6432692
 * Curso: Bacharelado em Ciências da Computação
 * EP 01 -- Simulação Concorrente de uma prova de 'Iron Man'
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
#include <string.h>
#include <unistd.h>

#include "defines.h"


/* Função que calcula um tempo aleatório que tem valor minimo de min e máximo de max. */
int timeRand(int min, int max){
	return (PRECISAO*min + PRECISAO*rand()/RAND_MAX*(max-min));
}

/* As funções a seguir calculam o tempo gasto em cada etapa para um Atleta a.	*
*  Dependendo da etapa deve ser passado um parametro extra para a função. 		*/
void natacao(Atleta a){
	if( a->sexo == FEMININO && a->categoria == AMADOR )
		a->ms[NATACAO] += timeRand(150, 300);
	if( a->sexo == MASCULINO && a->categoria == AMADOR )
		a->ms[NATACAO] += timeRand(120, 240);
	if( a->sexo == FEMININO && a->categoria == PROFISSIONAL )
		a->ms[NATACAO] += timeRand(90, 120);
	if( a->sexo == MASCULINO && a->categoria == PROFISSIONAL )
		a->ms[NATACAO] += timeRand(75, 100);
}

void corrida(Atleta a){
	if( a->sexo == FEMININO && a->categoria == AMADOR )
		a->ms[NATACAO] += timeRand(330, 420);
	if( a->sexo == MASCULINO && a->categoria == AMADOR )
		a->ms[NATACAO] += timeRand(300, 420);
	if( a->sexo == FEMININO && a->categoria == PROFISSIONAL )
		a->ms[NATACAO] += timeRand(260, 290);
	if( a->sexo == MASCULINO && a->categoria == PROFISSIONAL )
		a->ms[NATACAO] += timeRand(240, 260);
}

/* Para esta função o tipo de terreno também deve ser passado como parametro de entrada. */
void ciclismo( Atleta a, int terreno){
	switch(terreno){
		case SUBIDA:
			if( a->sexo == FEMININO && a->categoria == AMADOR )
				a->ms[CICLISMO] += timeRand(3600/10, 3600/15);
			if( a->sexo == MASCULINO && a->categoria == AMADOR )
				a->ms[CICLISMO] += timeRand(3600/12, 3600/20);
			if( a->sexo == FEMININO && a->categoria == PROFISSIONAL )
				a->ms[CICLISMO] += timeRand(3600/20, 3600/30);
			if( a->sexo == MASCULINO && a->categoria == PROFISSIONAL )
				a->ms[CICLISMO] += timeRand(3600/30, 3600/40);
			break;
		case PLANO:
			if( a->sexo == FEMININO && a->categoria == AMADOR )
				a->ms[CICLISMO] += timeRand(3600/25, 3600/40);
			if( a->sexo == MASCULINO && a->categoria == AMADOR )
				a->ms[CICLISMO] += timeRand(3600/30, 3600/45);
			if( a->sexo == FEMININO && a->categoria == PROFISSIONAL )
				a->ms[CICLISMO] += timeRand(3600/45, 3600/50);
			if( a->sexo == MASCULINO && a->categoria == PROFISSIONAL )
				a->ms[CICLISMO] += timeRand(3600/50, 3600/55);
			break;
		case DESCIDA:
			if( a->sexo == FEMININO && a->categoria == AMADOR )
				a->ms[CICLISMO] += timeRand(3600/50, 3600/65);
			if( a->sexo == MASCULINO && a->categoria == AMADOR )
				a->ms[CICLISMO] += timeRand(3600/50, 3600/70);
			if( a->sexo == FEMININO && a->categoria == PROFISSIONAL )
				a->ms[CICLISMO] += timeRand(3600/70, 3600/90);
			if( a->sexo == MASCULINO && a->categoria == PROFISSIONAL )
				a->ms[CICLISMO] += timeRand(3600/100, 3600/100);
			break;
	}
}

/* Para esta função é preciso especificar o numero da transicao no segundo parametro. */
void transicao( Atleta a, int t){
	if(t==T1){
		if( a->sexo == FEMININO && a->categoria == AMADOR )
			a->ms[T1] += timeRand(150, 390);
		if( a->sexo == MASCULINO && a->categoria == AMADOR )
			a->ms[T1] += timeRand(150, 330);
		if( a->sexo == FEMININO && a->categoria == PROFISSIONAL )
			a->ms[T1] += timeRand(90, 150);
		if( a->sexo == MASCULINO && a->categoria == PROFISSIONAL )
			a->ms[T1] += timeRand(90, 150);
	}
	else{
		if( a->sexo == FEMININO && a->categoria == AMADOR )
			a->ms[T2] += timeRand(270, 720);
		if( a->sexo == MASCULINO && a->categoria == AMADOR )
			a->ms[T2] += timeRand(240, 600);
		if( a->sexo == FEMININO && a->categoria == PROFISSIONAL )
			a->ms[T2] += timeRand(150, 360);
		if( a->sexo == MASCULINO && a->categoria == PROFISSIONAL )
			a->ms[T2] += timeRand(120, 180);
	}
}