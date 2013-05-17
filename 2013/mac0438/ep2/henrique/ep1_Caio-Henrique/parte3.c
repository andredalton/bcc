#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_THREADS 16

double step;
double part_sum[MAX_THREADS];
pthread_t threads[MAX_THREADS];
pthread_attr_t thread_attrs[MAX_THREADS];

long start_i[MAX_THREADS + 1];

void* worker_function(void* data) {
    long id = (long) data;
    register double sum = 0.0;
    register long i = start_i[id];
    long target = start_i[id + 1];
    register double ss = (step * step);
    register double x;

    for (; i < target; ++i) {
        x = (i*i + i + 0.25);
        sum += 4.0/(1.0+x*ss);
    }
    part_sum[id] = sum;

    return NULL;
}

int main(int argc, char *argv[]) {
    register long num_steps = atoi(argv[1]);
    long i;
    double pi, sum = 0.0;
    long num_cores = sysconf( _SC_NPROCESSORS_ONLN );
    long worker_share = num_steps / num_cores;

    step = 1.0/num_steps;

    /* Dividindo os passos entre as threads. */
    start_i[0] = 0;
    for(i = 1; i < num_cores; ++i)
        start_i[i] = start_i[i-1] + worker_share;
    start_i[num_cores] = num_steps;

    /* Criando as threads. */
    for(i = 0; i < num_cores; ++i) {
        pthread_attr_init(thread_attrs + i);
        pthread_attr_setdetachstate(thread_attrs + i, PTHREAD_CREATE_JOINABLE);
        pthread_create(threads + i, thread_attrs + i, worker_function, (void*)i);
    }

    /* Esperando elas terminarem. */
    for(i = 0; i < num_cores; ++i) {
        pthread_join(threads[i], NULL);
        sum += part_sum[i];
    }
    pi = step * sum;
    printf("Pi = %.15g\n", pi);

    for(i = 0; i < num_cores; ++i) {
        pthread_attr_destroy(thread_attrs + i);
    }
    return 0;
}
