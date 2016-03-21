/*
Nome: André Meneghelli Vale
NUSP: 4898948
Problema P2 - 1647. Divide an Island!
*/
#include<iostream>
#include<math.h>
#include<stdio.h>

using namespace std;

void rotacao(double *A, double *B, double *C) {
  double tmp;
  tmp = *A;
  *A = *B;
  *B = *C;
  *C = tmp;
}

int main() {
  double Ax, Ay, Bx, By, Cx, Cy;
  double Da, Db, Dc, tmp;
  double Perimetro, prod, Area, delta;
  int resp = 0;
  double r1, r2, s1, s2, p, q, k, l;

  cin >> Ax >> Ay >> Bx >> By >> Cx >> Cy;

  // Calculando as distâncias
  Da = sqrt ((Cx-Bx)*(Cx-Bx)+(Cy-By)*(Cy-By));
  Db = sqrt ((Cx-Ax)*(Cx-Ax)+(Cy-Ay)*(Cy-Ay));
  Dc = sqrt ((Ax-Bx)*(Ax-Bx)+(Ay-By)*(Ay-By));

  // Calculando o perímetro
  Perimetro = Da + Db + Dc;
  // Calculando a área
  Area = sqrt (Perimetro/2*(Perimetro/2-Da)*(Perimetro/2-Db)*(Perimetro/2-Dc));

  // Percorrendo todas as possibilidades de arranjos até encontrar uma resposta.
  for (int i = 0; i < 3 && resp == 0; i++) {
    prod = Da * Db / 2;

    // Calculando Delta e arredondando para zero valores pequenos
    delta = Perimetro/2*Perimetro/2 - 4*prod;
    if (fabs(delta) <= 1e-10)
      delta = 0;

    // Caso tenha raiz
    if (delta >= 0) {
      r1 = (Perimetro/2 + sqrt(delta))/2;
      r2 = (Perimetro/2 - sqrt(delta))/2;

      s1 = Perimetro/2 - r1;
      s2 = Perimetro/2 - r2;

      if (r1 <= Db+1e-10 && s1 <= Da+1e-10) {
        p = Cx + (r1/Db) * (Ax-Cx);
        q = Cy + (r1/Db) * (Ay-Cy);
        k = Cx + (s1/Da) * (Bx-Cx);
        l = Cy + (s1/Da) * (By-Cy);
        resp = 1;
      }
      else if (r2 <= Db+1e-10 && s2 <= Da+1e-10) {
        p = Cx + (r2/Db) * (Ax-Cx);
        q = Cy + (r2/Db) * (Ay-Cy);
        k = Cx + (s2/Da) * (Bx-Cx);
        l = Cy + (s2/Da) * (By-Cy);
        resp = 1;
      }
    }
    rotacao(&Da, &Db, &Dc);
    rotacao(&Ax, &Bx, &Cx);
    rotacao(&Ay, &By, &Cy);
  }

  if (!resp)
    cout << "NO";
  else
    printf("YES\n%.15lf %.15lf\n%.15lf %.15lf\n", p, q, k, l);

  return 0;
}
