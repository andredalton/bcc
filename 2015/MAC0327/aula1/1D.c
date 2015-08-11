/*******************************************************************************
 * Aluno:  André Meneghelli Vale,      Núm. USP: 4898948
 * Curso: Bacharelado em Ciências da Computação
 * Aula 1 - Desafio D
 * MAC0327 -- 10/07/2015 -- IME/USP, -- Prof. Cristina Gomes Fernandes
 * Compilador: gcc linux 4.8.4
 * Editor: Code Blocks;
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#define SIZE 100

int main() {
    char buffer[10];
    char *results;
    int n,
        i,
        spetya=0,
        sgena=0,
        gena,
        petya,
        max,
        ngames=0,
        *games[2],
        s,
        t;

    fgets(buffer, 10, stdin);
    n = atoi(buffer);
    results = (char *) malloc(n*sizeof(char));
    for(i=0; i<n; i++) {
        results[i] = '2' == getchar();
        if (results[i])
            sgena++;
        else
            spetya++;
        getchar();
    }

    games[0] = (int *) malloc(SIZE*sizeof(int));
    games[1] = (int *) malloc(SIZE*sizeof(int));

    max = (sgena>spetya) ? sgena : spetya;
    for (s=1; s<=max; s++) {
        for (t=(1+(n/(2*s-1)))/2; t<=n/s; t++) {
            if (t==0)
                continue;
            gena = petya = sgena = spetya = 0;
            for (i=0; i<n && sgena<s && spetya<s; i++) {
                if (results[i])
                    gena++;
                else
                    petya++;
                if ( gena==t ) {
                    sgena++;
                    gena = petya = 0;
                } else if ( petya==t ) {
                    spetya++;
                    gena = petya = 0;
                }
            }
            if (!petya && !gena && sgena!=spetya && (sgena==s || spetya==s) && i==n) {
                games[0][ngames] = s;
                games[1][ngames] = t;
                ngames++;
                if (ngames%SIZE==0) {
                    games[0] = (int *) realloc(games, (ngames+SIZE)*sizeof(int));
                    games[1] = (int *) realloc(games, (ngames+SIZE)*sizeof(int));
                }
            }
        }
    }

    printf("%d", ngames);
    for (i=0; i<ngames; i++)
        printf("\n%d %d", games[0][i], games[1][i]);

    free(games[0]);
    free(games[1]);
    return 0;
}
