#include <stdio.h>
#include <stdlib.h>

void heapsort(long int v[], int n) {
   int i = n / 2, pai, filho, t;

   for (;;) {
      if (i > 0) {
          i--;
          t = v[i];
      } else {
          n--;
          if (n == 0) return;
          t = v[n];
          v[n] = v[0];
      }

      pai = i;

      filho = i * 2 + 1;

      while (filho < n) {

          if ((filho + 1 < n)  &&  (v[filho + 1] > v[filho]))
              filho++;
          if (v[filho] > t) {
             v[pai] = v[filho];
             pai = filho;
             filho = pai * 2 + 1;
          } else {
             break;
          }
      }
      v[pai] = t;
   }
}

int main() {
    unsigned int *v;
    int N, n, i, pai, filho, t;

    scanf("%d", &n);
    N = n;
    v = (unsigned int *) malloc(n*sizeof(unsigned int));

    for ( i=0; i<n; i++) {
        scanf("%lu", v+i);
    }

   for (i=n/2;;) {
      if (i > 0) {
          i--;
          t = v[i];
      } else {
          n--;
          if (n == 0) break;
          t = v[n];
          v[n] = v[0];
      }

      pai = i;

      filho = i * 2 + 1;

      while (filho < n) {

          if ((filho + 1 < n)  &&  (v[filho + 1] > v[filho]))
              filho++;
          if (v[filho] > t) {
             v[pai] = v[filho];
             pai = filho;
             filho = pai * 2 + 1;
          } else {
             break;
          }
      }
      v[pai] = t;
   }

    if (N%2==0)
        printf("%.1f", ((float)v[N/2-1]+(float)v[N/2])/2);
    else
        printf("%lu", v[N/2]);

    free(v);
    return 0;
}
