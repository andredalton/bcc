#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h> /* Para trabalhar com threads */

#define N 100000000 /* 100 milhões = Número máximo de números primos */
#define M 25 /* Número de threads */
#define SLEEP 10 /* Tempo, em micro segundos, de espera em um Busy-Wait */

int *primos; /* Vetor onde serão guardados os números primos */
int n; /* Número de primos já calculados */
static int index; /* Índice no vetor primos do último primo visto */
double total; /* Constante de brun calculada até o momento */
int senha; /* Variáveis compartilhada para controlar as threads */
int barreira; /* Shared Count - Implementação de barreira */
float *buffer; /* Buffer onde são guardados os resultados das M-1 threads a cada passo de sincronização */

/*
 * Função que recebe um número p e retorna:
 * 1, se p é primo
 * 0, caso contrário
 * O algoritmo usado é o Crivo de Eratóstenes
 */
int crivo(int p) {
	int i, square;
	square = (int) sqrt(p);
	square++;

	if (p == 2) return 1;
	if (p <= 1 || p % 2 == 0) return 0;
	for(i = 3; i < square; i += 2)
		if (p % i == 0) return 0;
	return 1;
}

/*
 * Função que será usada por uma thread para gerar primos.
 */
void geraPrimos() { /* O 2 não está no vetor de primos...então primos[0] == 3. Pois 2 e 3 não são primos gêmeos. */
	int i;
	for (i = 3; ; i += 2) {
		if (crivo(i)) {
			if (n < N) {
				primos[n] = i;
				n++;
			}
			else {
				return;
			}
		}
	}
}

/*
 * Função que será usada por cada thread
 * tid - Apontador para o Id da thread
 */
void encontraGemeos(int *tid) {
	int i, id;
	double soma;

	soma = 0;
	id = *(tid);

	while(1) {
		while (senha != id || barreira == 0) usleep(SLEEP); /* Enquanto não é a vez da thread ou a barreira está desligada */

		/* Se a thread chegou aqui é porque senha == id E barreira == 1 */
		for (i = index; ; i++) { /* Roda até encontrar primos gêmeos */
			while (i >= n) {
				usleep(SLEEP);
			}
			if (primos[i] == primos[i - 1] + 2) {
				index = i + 1;
				break;
			}
		}
		senha++;
		/* Calcula a soma */
		soma = (1 / (double) primos[i - 1]) + (1 / (double) primos[i]);
		buffer[id - 1] = soma;

		if (senha == M) { /* Se é a última thread */
			barreira = 0; /* Desliga a barreira */
			senha = 1;
		}
	}
	return;
}

int main(int argc, char *argv[]) {
	int i;
	int * arg;
	float entrada;
	pthread_t * threadIds;

	/* Leitura da entrada */
	if (argc != 2) {
		printf("Entrada incorreta!\n");
		printf("Funcionamento:\n\n");
		printf("./ep <numero>\n");
		return -1;
	}
	entrada = atof(argv[1]);

	/* Inicializações */
	n = 0;
	index = 1;
	senha = 1;
	barreira = 1;
	total = 0;
	primos = (int *) malloc(N * sizeof(int));
	buffer = (float *) malloc((M - 1) * sizeof(float) );
	for (i = 0; i < M - 1; i++) {
		buffer[i] = 0;
	}

	threadIds = (pthread_t *) malloc(M * sizeof(pthread_t));
	if (!threadIds) {
		fprintf(stderr,"Erro no malloc do pthread_t\n");
		return(1);
	}
	arg = (int *) malloc(M * sizeof(int) );
	if (!arg) {
		free(threadIds);
		fprintf(stderr,"Erro no malloc do vetor\n");
		return(1);
	}

	/* Primeira Thread gera os primos com crivo */
	arg[0] = 0;
	if (pthread_create(&(threadIds[0]),NULL,(void *)geraPrimos,(void *)&(arg[0]))) {
		fprintf(stderr,"Erro no pthread_create\n");
		free(threadIds);
		free(arg);
		return(2);
	}
	/* Outras threads */
	for (i = 1; i < M; i++) {
		arg[i] = i;
		if (pthread_create(&(threadIds[i]),NULL,(void *)encontraGemeos,(void *)&(arg[i]))) {
			fprintf(stderr,"Erro no pthread_create\n");
			free(threadIds);
			free(arg);
			return(2);
		}
	}

	while (1) {
		/* Faz as threads acharem primos gêmeos */
		while(barreira) usleep(SLEEP); /* Enquanto a barreira está ligada (Espera as M-1 threads acabarem */
		for (i = 0; i < M - 1; i++) { /* Verifica os resultados das threads */
			total += buffer[i];
			if (total >= entrada) {
				printf("%f\n", total);
				return 1;
			}
		}
		for (i = 0; i < M - 1; i++) { /* Zera o buffer */
			buffer[i] = 0;
		}
		barreira = 1; /* Liga barreira */
	}

	return 0;
}
