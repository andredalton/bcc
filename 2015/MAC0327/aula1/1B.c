#include <stdio.h>
#include <stdlib.h>

#define SIZE 1000

struct segment
{
    char num;
    int end;
} typedef Segment;

int main() {
    int n,
        i,
        j,
        left,
        right,
        k,
        size = 0,
        nsegs = 1,
        cmp;

    char str[SIZE],
         tmp[SIZE],
         mem;
    Segment segs[2];

    scanf("%d", &n);
    scanf("%s", str);

    // Analisando inicio.
    mem = str[0];
    for (i=0; i<n && mem==str[i]; i++);
    for (j=0; j<n-i && mem==str[n-1-j]; j++);
    size = i+j;

    segs[0].num = str[i-1];
    segs[0].end = i-1;
    mem = str[i];

    // Continuando com a analise
    i=size+segs[0].end;
    while (i<n) {
        if (mem == str[i]) {
            for (j=1; j<size && mem==str[i-j]; j++);
            for (k=1; i+k<n && mem==str[i+k]; k++);
        } else {
            mem = str[i];
            for (j=1; j<size-1 && mem==str[i-j]; j++);
            for (k=1; i+k<n&& mem==str[i+k]; k++);
        }
        if (j+k-1>size) {
            // Novo tamanho
            size = j+k-1;
            nsegs = 1;
            segs[0].num = str[i+k-1];
            segs[0].end = i+k-1;
        } else if (j+k-1==size) {
            // Comparar
            nsegs++;
            segs[(nsegs+1)%2].num = str[i+k-1];
            segs[(nsegs+1)%2].end = i+k-1;
            cmp = 0;
            for (k=1; cmp==0 && i+k-size<n; k++) {
                cmp = (10+str[(segs[(nsegs-1)%2].end+k)%n]-segs[(nsegs-1)%2].num)%10;
                cmp -= (10+str[(segs[nsegs%2].end+k)%n]-segs[nsegs%2].num)%10;
            }

            if (cmp>=0) nsegs++;
        }
        i += size;
        mem = str[i];
    }

    // Transformando o resultado.
    for (i=0; i<size; i++) tmp[i] = '0';
    for (j=0; i+j<n; j++) {
        tmp[size+j] = str[(segs[(nsegs-1)%2].end+j+1)%n] - segs[(nsegs-1)%2].num + '0';
        if (tmp[size+j]<'0')
            tmp[size+j] += 10;
    }
    tmp[n] = '\0';

    printf("%s", tmp);
    return 0;
}
