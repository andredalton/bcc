#include <stdio.h>
#include <stdlib.h>

int bin_search(unsigned int array[], unsigned int search, int n) {
    int first = 0;
    int last = n - 1;
    int middle = (first+last)/2;

    while (first <= last) {
        if (array[middle] < search)
            first = middle + 1;
        else if (array[middle] == search) {
            break;
        }
        else
            last = middle - 1;

        middle = (first + last)/2;
    }
    return middle;
}

void insert(unsigned int v[], int n, unsigned int value) {
    int i, N = bin_search(v, value, n/2+1);

    if (v[N] > value)
        N--;

    for (i=0; i<N; i++)
        v[i] = v[i+1];
    if (N >= 0)
        v[N] = value;
}

int main() {
    unsigned int *v, tmp;
    int n, i;

    scanf("%d", &n);
    v = (unsigned int *) malloc((n/2+1)*sizeof(unsigned int));

    for ( i=0; i<n/2+1; i++) {
        v[i] = 0;
    }

    for ( i=0; i<n; i++) {
        scanf("%u", &tmp);
        insert(v, n, tmp);
    }

    if (n%2==0){
        printf("%.1lf\n", ((double)v[0]+v[1])/2);
    }
    else
        printf("%u.0\n", v[0]);

    free(v);
    return 0;
}
