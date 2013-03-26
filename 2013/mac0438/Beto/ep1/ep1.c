/* Alberto Bueno Júnior
 * Felipe Solferini
 * Daniel Reverbel
 *
 * EP1 - Programação concorrente
 * 
 * Compilando:  gcc -Wall -pthread ep1.c -o ep1
 * Rodando: ./ep1 <m> <n> [s]
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> /* Para trabalhar com threads */
#include <sys/time.h>
#include <unistd.h>

#define COPY_RATE 0.75 /* bytes / ms */
#define MIN_CHEGADA 250 /* ms */
#define MAX_CHEGADA 1000 /* ms */
#define MAX_TAMANHO 1500 /* bytes */
#define CONV 10

struct pacote {
	int t_chegada;
	int tamanho;
};
typedef struct pacote* pacote_pointer;

struct node {
	pacote_pointer pacote;
	struct node* next;
};
typedef struct node * node_pointer;

struct dados {
	int pac_copiados;
	int b_copiados;
	double ociosidade_total;
	double tempo_total;
};
typedef struct dados *copiador_dados;

/* Variáveis globais */
int senha;
int copiados;
node_pointer head;
node_pointer tail;
copiador_dados *cop_dados;
static pthread_mutex_t cs_mutex = PTHREAD_MUTEX_INITIALIZER;

/* Cria novo nó, para depois ser colocado na fila */
node_pointer novo_node() {
	node_pointer novo;
	novo = (node_pointer) malloc(sizeof(* novo) );
	if (novo == NULL) {
		perror("Malloc ERROR no novo");
		exit(1);
	}
	novo->pacote = (pacote_pointer) malloc(sizeof(*novo->pacote) );
	novo->pacote->t_chegada = MIN_CHEGADA + (rand() % (MAX_CHEGADA - MIN_CHEGADA) );
	novo->pacote->tamanho = 1 + (rand() % MAX_TAMANHO);
	novo->next = NULL;
	return novo;
}
/* Verifica se a fila está vazia */
int fila_vazia() {
	pthread_mutex_lock( &cs_mutex );
	if (head->next == NULL) {
		pthread_mutex_unlock( &cs_mutex );
		return 1;
	}
	else {
		pthread_mutex_unlock( &cs_mutex );
		return 0;
	}
}

/* Função que será usada nas threads 'copiador' */
void * copiador (int *n) {
	struct timeval t_antes, t_depois, t_final;
	double delta;
	node_pointer prox;
	float espera;
	while(1) {
		gettimeofday(&t_antes, NULL);
		while(fila_vazia()) {
			usleep(MIN_CHEGADA * 500);
		}
		gettimeofday(&t_depois, NULL);
		delta = (t_depois.tv_sec - t_antes.tv_sec) + (t_depois.tv_usec - t_antes.tv_usec) / 1000000; /* s */
		/* Início da seção crítica */
		pthread_mutex_lock( &cs_mutex );
		prox = head->next;
		if (prox == NULL) {
			pthread_mutex_unlock( &cs_mutex );
			continue;
		}
		head->next = prox->next;
		if (prox == tail) {
			tail = head;
		}
		pthread_mutex_unlock( &cs_mutex );
		/* Final da seção crítica */
		espera = prox->pacote->tamanho / (float) COPY_RATE;
		usleep((espera * 1000) / CONV);
		cop_dados[*n]->b_copiados += prox->pacote->tamanho;
		cop_dados[*n]->pac_copiados += 1;
		cop_dados[*n]->ociosidade_total += delta;
		gettimeofday(&t_final, NULL);
		delta = (t_final.tv_sec - t_antes.tv_sec) + (t_final.tv_usec - t_antes.tv_usec) / 1000000; /* s */
		cop_dados[*n]->tempo_total += delta;
		copiados++;
	}

	return NULL;
}

/* Função que será usada nas threads 'pacote' */
void * pacote (int *n) {
	node_pointer novo;

	/* Criando pacote */
	novo = novo_node();
	/* Espera a sua vez */
	while (senha != *n) {
		usleep((MIN_CHEGADA * 100) / CONV);
	}
	/* Rodando o processo */
	usleep((novo->pacote->t_chegada * 1000) / CONV);
	/* Início da seção crítica */
	pthread_mutex_lock( &cs_mutex );
	novo->next = NULL;
	tail->next = novo;
	tail = novo;
	senha++;
	pthread_mutex_unlock( &cs_mutex );
	/* Final da seção crítica */
	return NULL;
}

int main (int argc, char *argv[]) {
	/* ====== Declarações ====== */
	int i, m, n, seed, script_out;
	pthread_t * ids_pacotes; /* Guardam os ids das threads */
	pthread_t * ids_copiadores;
	int * arg; /* Vetor com os argumentos passados para cada thread */
	/* m = quantidade total de pacotes a serem lidos */
	/* n = quantidade de pacotes que podem ser copiados simultaneamente */

	/* ====== Inicializações ====== */
	if (argc == 3) {
		m = atoi(argv[1]);
		n = atoi(argv[2]);
		script_out = 0;
	}
	else {
		if (argc == 4) {
			script_out = 1;
			m = atoi(argv[1]);
			n = atoi(argv[2]);
		}
		else {
			script_out = 0;
			printf("Entrada incorreta!\n");
			return -1;
		}
	}
	seed = time(NULL);
	srand(seed);
	senha = 0;
	head = (node_pointer) malloc(sizeof(*head) );
	if (head == NULL) {
		perror("Malloc ERROR no HEAD");
		exit(1);
	}
	head->pacote = NULL;
	head->next = NULL;
	tail = head;
	cop_dados = (copiador_dados *) malloc(n * sizeof(copiador_dados) );
	if (cop_dados == NULL) {
		perror("Malloc ERROR no cop_dados");
		exit(1);
	}
	for (i = 0; i < n; i++) {
		cop_dados[i] = (copiador_dados) malloc(sizeof(struct dados) );
		if (cop_dados[i] == NULL) {
			perror("Malloc ERROR no cop_dados[i]");
			exit(1);
		}
		cop_dados[i]->b_copiados = 0;
		cop_dados[i]->ociosidade_total = 0;
		cop_dados[i]->tempo_total = 0;
		cop_dados[i]->pac_copiados = 0;
	}

	/* ====== Malloca os vetores ====== */
	ids_pacotes = (pthread_t *) malloc(m * sizeof(pthread_t));
	if (ids_pacotes == NULL) {
		perror("Malloc ERROR no ids_pacotes");
		exit(1);
	}
	if (!ids_pacotes) {
		fprintf(stderr,"Erro no malloc do pthread_t\n");
		return(1);
	}
	ids_copiadores = (pthread_t *) malloc(n * sizeof(pthread_t));
	if (!ids_copiadores) {
		fprintf(stderr,"Erro no malloc do pthread_t\n");
		return(1);
	}
	arg = (int *) malloc(m * sizeof(int) );
	if (!arg) {
		free(ids_pacotes);
		free(ids_copiadores);
		fprintf(stderr,"Erro no malloc do vetor\n");
		return(1);
	}

	/* ====== Inicializa os copiadores ====== */
	for (i = 0; i < n; i++) {
		arg[i] = i;
		if (pthread_create(&(ids_copiadores[i]),NULL,(void *)copiador,(void *)&(arg[i]))) {
			fprintf(stderr,"Erro no pthread_create\n");
			free(ids_pacotes);
			free(ids_copiadores);
			free(arg);
			return(2);
		}
	}
	for (i = 0; i < m; i++) {
		arg[i] = i;
		if (pthread_create(&(ids_pacotes[i]),NULL,(void *)pacote,(void *)&(arg[i]))) {
			fprintf(stderr,"Erro no pthread_create\n");
			free(ids_pacotes);
			free(ids_copiadores);
			free(arg);
			return(2);
		}
	}
	/* ====== LOOP PRINCIPAL ====== */
	while(copiados < m) {
		usleep(1000);
	}

	for (i = 0; i < n; i++) {
		if (script_out) {
			printf("%d %d %d %.2f %.2f\n", i + 1, cop_dados[i]->pac_copiados, cop_dados[i]->b_copiados, cop_dados[i]->ociosidade_total, cop_dados[i]->tempo_total);
		}
		else {
			printf("C%d: %d pacotes, %d bytes, %.2f s (total = %.2f s)\n", i + 1, cop_dados[i]->pac_copiados, cop_dados[i]->b_copiados, cop_dados[i]->ociosidade_total, cop_dados[i]->tempo_total);
		}
	}

	free(ids_copiadores);
	free(ids_pacotes);
	return(0);
}
