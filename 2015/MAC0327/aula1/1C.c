/*******************************************************************************
 * Aluno:  André Meneghelli Vale,      Núm. USP: 4898948
 * Curso: Bacharelado em Ciências da Computação
 * Aula 1 - Desafio C
 * MAC0327 -- 08/07/2015 -- IME/USP, -- Prof. Cristina Gomes Fernandes
 * Compilador: gcc linux 4.8.4
 * Editor: Code Blocks;
 ******************************************************************************/

#include <stdio.h>

#define SIZE 100

struct list
{
    int lst[SIZE];
    int pos;
} typedef List;

void start_list(List *lst) {
    lst->pos = -1;
}

void add_element(int e, List *lst) {
    lst->lst[++lst->pos] = e;
}

int line_cmp(int lin, int col, int cols, char mtz[][SIZE], char deleted[], List used[]) {
    int i,
        j,
        res;

    for ( i=col; i<cols; i++) {
        if (deleted[i]) continue;
        if (mtz[lin+1][i]<mtz[lin][i]) {
            deleted[i] = 1;
            for ( j=0; j<=used[i].pos; j++) {
                res = -1;
                while (res<0) {
                    res = line_cmp(used[i].lst[j], i+1, cols, mtz, deleted, used);
                    if (res>0) add_element(i, &used[res]);
                }
            }
        }
        if (mtz[lin+1][i]>mtz[lin][i]) return i;
    }
    return SIZE+1;
}

int main() {
    int lins,
        cols,
        i,
        j,
        del = 0,
        res;

    char mtz[SIZE][SIZE],
         deleted[SIZE];

    List used[SIZE];

    scanf("%d %d", &lins, &cols);
    for (i=0; i<lins; i++)
        scanf("%s", mtz[i]);

    for (i=0; i<cols; i++) {
        start_list(&used[i]);
        deleted[i] = 0;
    }

    /* Inicializando recursão pra todas as linhas. */
    for (i=0; i<lins-1; i++) {
        res = -1;
        while (res<0) {
            res = line_cmp(i, 0, cols, mtz, deleted, used);
            if (res<SIZE+1) add_element(i, &used[res]);
        }
    }

    for (j=0; j<cols; j++) del += deleted[j];

    printf("%d", del);
    return 0;
}
