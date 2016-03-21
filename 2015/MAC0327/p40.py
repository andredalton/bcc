def main():
    n, m, k = map(int, raw_input().split())
    array = map(int, raw_input().split())
    operacoes = []

    for i in range(m):
        operacoes.append(map(int, raw_input().split()))



if __name__ == '__main__':
    main()



#include<bits/stdc++.h>

using namespace std;

/******************************************************************************/

class operation {
public:
	long long int l;
	long long int r;
	long long int d;
};

int main() {
	memset( op, 0, MAXX * sizeof (operation));
	memset( a, 0, MAXX * sizeof (long long int));
	memset( difa, 0, MAXX * sizeof (long long int));
	memset( difo, 0, MAXX * sizeof (long long int));
	long long int n, m, k;
	cin >> n >> m >> k;
	cin >> a[0];
	for (long long int i = 1; i < n; i++) {
		cin >> a[i];
	}
	for (long long int i = 0; i < m; i++) {
		long long int l, r;
		cin >> l >> r >> op[i].d;
		op[i].l = --l;
		op[i].r = --r;
	}
	for (long long int i = 0; i < k; i++) {
		long long int x, y;
		cin >> x >> y;
		--x;
		--y;
		difo[x]++;
		if (y != m - 1) {
			difo[y + 1]--;
		}
	} // aqui difo[] e' o vet diferenca da freq de uso das operacoes op[]

	difa[op[0].l] += difo[0] * op[0].d;
	if (op[0].r != n - 1) {
		difa[op[0].r + 1] -= difo[0] * op[0].d;
	}
	for (long long int i = 1; i < m; i++) {
		difo[i] += difo[i - 1]; // iterativamente recupera o vetor op atualizado (onde difo[0] == op[0] atualizado)
		difa[op[i].l] += difo[i] * op[i].d;
		if (op[i].r != n - 1) {
			difa[op[i].r + 1] -= difo[i] * op[i].d;
		}
	}

	cout << a[0] + difa[0];
	for (long long int i = 1; i < n; i++) {
		difa[i] += difa[i - 1];
		cout << " " << a[i] + difa[i];
	}
	cout << endl;
	return 0;
}