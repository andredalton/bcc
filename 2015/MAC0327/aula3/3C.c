/*******************************************************************************
 * Aluno:  André Meneghelli Vale,      Núm. USP: 4898948
 * Curso: Bacharelado em Ciências da Computação
 * Aula 3 - Desafio B
 * MAC0327 -- 11/07/2015 -- IME/USP, -- Prof. Cristina Gomes Fernandes
 * Compilador: gcc linux 4.8.4
 * Editor: Code Blocks;
 ******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdio.h>

#define MAX 100
#define INT 9

struct VeryLongInt {
    int n;
    int nums[MAX+2];
} typedef veryLongInt;

int main() {
    char buffer[MAX+100],
         mem;
    int n, len;
    veryLongInt mtz[MAX];

    while (fgets(buffer,MAX, stdin)) {
        n = 0;
        mtz[n].n = 0;
        printf("\nbuf = %s\n\n", buffer);
        for ( n=strlen(buffer); n>=0; n-=INT) {
            printf(" - %d\n", atoi(&buffer[n-INT]));
        }
        printf(" = %d\n", atoi(&buffer[n-INT]));
        n++;
    }

    return 0;
}
