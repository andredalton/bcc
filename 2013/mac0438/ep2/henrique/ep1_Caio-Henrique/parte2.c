#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <unistd.h>

static unsigned long num_steps;
double step;
int main (int argc, char **argv)
{
	unsigned long int i, cores;
	double x, pi, sum = 0.0;

	cores = sysconf( _SC_NPROCESSORS_ONLN );

	num_steps = atol(argv[1]);

	step = 1.0/(double) num_steps;
	omp_set_num_threads(cores);

	/*
	 * a linha abaixo diz para paralelizar o for subsequente (openmp
	 * trata de quebrar o for e repartir entre as threads seguindo
	 * o agendamento guiado, schedule(guided)).
	 * reduction(+:sum) faz com que a variavel sum seja tratada como
	 * privada em cada thread evitando que haja concorrencia e
	 * a thread principal cuida, apos o laco, de atualizar o conteudo
	 * de sum realizando a operacao de adicao (op:var) entre a sum
	 * de todas as threads.
	 *
	 * private(x) faz com que cada thread tenha sua propria variavel x
	 * e que nao e' compartilhada. Isto pode ser feito pois x e' uma
	 * variavel independente a cada laco; auxiliar e temporaria.
	 *
	 * */
	#pragma omp parallel for reduction(+:sum) private(x) schedule(guided)
	for (i=1;i<= num_steps; i++)
	{
		x = (i-0.5)*step;
		sum = sum + 4.0/(1.0+x*x);
	}

	pi = step * sum;
	printf("Pi = %.15g\n", pi);
	return 1;
}
