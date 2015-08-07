#include <stdio.h>
#include <stdlib.h>

#define SIZE 100

int main() {
    int lin,
        col,
        i,
        j,
        del = 0;

    char mtz[SIZE][SIZE],
         used[SIZE];

    scanf("%d %d", &lin, &col);

    for (i=0; i<lin; i++)
        scanf("%s", mtz[i]);

    for (i=0; i<col; i++)
        used[i] = 1;

    for (i=1; i<lin; i++) {
        for (j=0; j<col; j++) {
            printf("\n[%d, %d] - \n", i, j);
            if (!used[j]) continue;
            printf("%c - %c = %d\t\t(%d, %d)\n", mtz[i-1][j], mtz[i][j], mtz[i-1][j]-mtz[i][j], i, j);
            if (mtz[i-1][j]<mtz[i][j]) {
                printf("\ndel");
                used[j] = 0;
                del++;
            }
        }
    }

    for (j=0; j<col; j++)
        printf("%d ", used[j]);

    printf("\n\n%d", del);
    return 0;
}
