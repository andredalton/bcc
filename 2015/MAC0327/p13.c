/*******************************************************************************
 * Aluno:  André Meneghelli Vale,      Núm. USP: 4898948
 * Curso: Bacharelado em Ciências da Computação
 * Aula 3 - Desafio B
 * MAC0327 -- 12/07/2015 -- IME/USP, -- Prof. Cristina Gomes Fernandes
 * Compilador: gcc linux 4.8.4
 * Editor: Code Blocks;
 ******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#define MAX 102
#define INT 9
#define LONG_ORDER 1000000000

int main() {
    char buffer[MAX];
    int n=0, m, i, j;
    unsigned long int sum[1 + MAX/INT],
        actual;

    for (i=0; i<1+MAX/INT; i++)
        sum[i] = 0;

    if ( fgets(buffer,MAX, stdin) ) {
        for ( i=strlen(buffer)-1; i>=0; i-=INT) {
            if (i-INT<0) {
                sum[n] = atoi(&buffer[0]);
            }
            else {
                sum[n] = atoi(&buffer[i-INT]);
                buffer[i-INT] = 0;
            }
            n++;
        }
    }

    while (fgets(buffer,MAX, stdin)) {
        m = 0;
        for ( i=strlen(buffer)-1; i>=0; i-=INT) {
            if (i-INT<0) {
                actual = atoi(&buffer[0]);
            }
            else {
                actual = atoi(&buffer[i-INT]);
                buffer[i-INT] = 0;
            }
            sum[m] += actual;
            if (sum[m]>=LONG_ORDER) {
                sum[m+1]++;
                sum[m] %= LONG_ORDER;
                if (m==n) n++;
            }
            m++;
        }
    }

    printf("%lu", sum[n-1]);
    for ( n--; n>0; n--)
        printf("%09lu", sum[n-1]);

    printf("\n");
    return 0;
}
