/*******************************************************************************
 * Aluno:  André Meneghelli Vale,      Núm. USP: 4898948
 * Curso: Bacharelado em Ciências da Computação
 * Aula 1 - Desafio A
 * MAC0327 -- 08/07/2015 -- IME/USP, -- Prof. Cristina Gomes Fernandes
 ******************************************************************************/

#include <stdio.h>

int main() {
    int tmp;
    int max = 1000;
    int min = 0;
    int i;
    int n;
    int v[100];

    scanf("%d", &n);
    for(i=0; i<n; i++) {
        scanf("%d", &v[i]);
        tmp = v[i]-v[i-2];
        if (i>=2 && tmp<max)
            max = tmp;
        tmp = v[i]-v[i-1];
        if (i>=2 && tmp>min)
            min = tmp;
    }
    if(min>max && n>3)
        max = min;
    printf("%d", max);
}
