#include <iostream>
#include <cmath>
#define EP 1e-6

using namespace std;

struct Ponto {
    double x, y;

    Ponto() {}
    Ponto(double x, double y) : x(x), y(y) {}

    double distancia(Ponto A) {
        return sqrt(pow(A.x-x,2)+pow(A.y-y,2));
    }
};

struct Circulo {
    Ponto c; double r;
    Circulo(Ponto c, double r) : c(c), r(r) { }

    Circulo(Ponto p1, Ponto p2, double r) {
        double d2 = (p1.x - p2.x) * (p1.x - p2.x) +
                    (p1.y - p2.y) * (p1.y - p2.y);
        double det = r*r / d2 - 0.25;
        double h = sqrt(det);

        this->c = Ponto((p1.x + p2.x) * 0.5 + (p1.y - p2.y) * h,
                        (p1.y + p2.y) * 0.5 + (p2.x - p1.x) * h);
        this->r = r;
    }


    static bool invalido(Ponto p1, Ponto p2, double r) {
        double d2 = (p1.x - p2.x) * (p1.x - p2.x) +
                    (p1.y - p2.y) * (p1.y - p2.y);
        double det = r*r / d2 - 0.25;
        return det < 0.0;
    }


    bool dentro(Ponto p) {
        return c.distancia(p)-r < 1e-6;
    }
};

int best(Circulo c1, int n, Ponto *P) {
    int sum1 = 0;
    for(int k=0; k<n; k++) {
        if (c1.dentro(P[k])) sum1++;
    }
    return sum1;
}

int main() {
    int n, r, R;
    Ponto P[106];
    while(cin >> n) {
        for(int i=0; i<n; i++) cin >> P[i].x >> P[i].y;
        cin >> R >> r;
        R-=r;

        int maxx = 0;
        for(int i=0; i<n; i++) {
            for(int j=0; j<n; j++) {
                if (i==j) maxx = max(maxx, best(Circulo(P[i], R), n, P));
                else if (!Circulo::invalido(P[i], P[j], R)) maxx = max(maxx, best(Circulo(P[i], P[j], R), n, P));
            }
        }
        cout << maxx << endl;
    }
}