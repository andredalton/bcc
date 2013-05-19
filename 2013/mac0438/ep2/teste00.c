#include <stdio.h>
#include <math.h>

int main( ) {
   int i;
   double num = 1,
          denum = 7,
          res[10];

   res[0] = num / denum;
   printf("%.100f\n", res[0]);
   for (i = 0; i < 10; i++) {
      num *= 1e+7;
      num -= denum * ( (int) (num / denum) );
      res[i] = num / denum;
      res[i] *= pow( 1e-7, i + 1);
      printf("%.100f\n", res[i]);
   }
   return 0;
}