/*******************************************************************************
 * Aluno:  André Meneghelli Vale,      Núm. USP: 4898948
 * Curso: Bacharelado em Ciências da Computação
 * Aula 3 - Desafio B
 * MAC0327 -- 11/07/2015 -- IME/USP, -- Prof. Cristina Gomes Fernandes
 * Compilador: gcc linux 4.8.4
 * Editor: Code Blocks;
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#define MAX 33

int walk(char maze[][MAX], int lin, int col, int n) {
    int walls = 0;

    if (!maze[lin][col]) {
        maze[lin][col] = 2;

        if (lin<n-1) {
            if (maze[lin+1][col]==1) walls++;
            else if (maze[lin+1][col]!=2)
                walls += walk(maze, lin+1, col, n);
        }
        if (lin>0)  {
            if (maze[lin-1][col]==1) walls++;
            else if (maze[lin-1][col]!=2)
                walls += walk(maze, lin-1, col, n);
        }
        if (col<n-1) {
            if (maze[lin][col+1]==1) walls++;
            else if (maze[lin][col+1]!=2)
                walls += walk(maze, lin, col+1, n);
        }
        if (col>0)  {
            if (maze[lin][col-1]==1) walls++;
            else if (maze[lin][col-1]!=2)
                walls += walk(maze, lin, col-1, n);
        }
        if (lin==0 && col!=0) walls++;
        else if (lin==n-1 && col!=n-1) walls++;
        if (col==0 && lin!=0) walls++;
        else if (col==n-1 && lin!=n-1) walls++;
    }
    return walls;
}

int main() {
    char buffer[MAX+1],
         maze[MAX][MAX],
         n;
    int
         lin,
         col;

    fgets(buffer, 10, stdin);
    n = atoi(buffer);
    for ( lin=0; lin<n; lin++) {
        fgets(buffer, MAX+1, stdin);
        for ( col=0; col<n; col++) {
            maze[lin][col] = buffer[col]=='#';
        }
    }

    printf("%d", 9*(walk(maze, 0, 0, n)+walk(maze, n-1, n-1, n)));
    return 0;
}
