#include <omp.h> 
#include <stdio.h>

static long num_steps = 1000000;
double step;

int main() {
    int i,
        id,
        nthreads;
    double
        x,
        sum,
        pi=0.0;
    
    step = 1.0/(double) num_steps; 
    
    #pragma omp parallel private (i, id, x, sum)
    {   
        id = omp_get_thread_num();
        sum=0.0;
        nthreads = omp_get_num_threads();
        for (i=id; i<num_steps; i+=nthreads){ 
            x = (i+0.5)*step;
            sum += 4.0/(1.0+x*x);
        }
        #pragma omp critical
        pi += step*sum;
    }
    printf("PI: %g\n", pi);

    return 0;
}