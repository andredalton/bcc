/*******************************************************************************
 * Aluno:  André Meneghelli Vale,      Núm. USP: 4898948
 * Curso: Bacharelado em Ciências da Computação
 * Aula 3 - Desafio A
 * MAC0327 -- 10/07/2015 -- IME/USP, -- Prof. Cristina Gomes Fernandes
 * Compilador: gcc linux 4.8.4
 * Editor: Code Blocks;
 ******************************************************************************/

#include <stdio.h>

int main() {
    int i,
        test,
        farm;
    unsigned long int bigBudge, size, budge, animals;

    scanf("%d", &test);
    for(i=0; i<test; i++) {
        scanf("%d", &farm);
        bigBudge = 0;
        for(; farm>0; farm--) {
            scanf("%lu %lu %lu", &size, &animals, &budge);
            if (animals) bigBudge += size*budge;
        }
        printf("%lu\n", bigBudge);
    }
    return 0;
}
