#include<bits/stdc++.h>

using namespace std;

typedef struct Operacao {
    long long int l;
	long long int r;
	long long int d;
} operacao;

static const long long int MAXX = 1e5 + 1;

void le_operacao(operacao &op) {
    cin >> op.l >> op.r >> op.d;
	--(op.l);
	--(op.r);
}

int main() {
    operacao op[MAXX];
    long long int a[MAXX];
    long long int variacao[MAXX];
    long long int uso[MAXX];

	memset( op, 0, MAXX * sizeof (operacao));
	memset( a, 0, MAXX * sizeof (long long int));
	memset( variacao, 0, MAXX * sizeof (long long int));
	memset( uso, 0, MAXX * sizeof (long long int));
	long long int n, m, k;
	cin >> n >> m >> k;
	// Lendo o array inicial.
	for (long long int i = 0; i < n; i++) cin >> a[i];
	for (long long int i = 0; i < m; i++) le_operacao(op[i]);

	// Lendo os usos das operações
	for (long long int i = 0; i < k; i++) {
		long long int x, y;
		cin >> x >> y;
		uso[--x]++;
		if (--y != m - 1) uso[y + 1]--;
	}

	variacao[op[0].l] += uso[0] * op[0].d;
	if (op[0].r != n - 1) variacao[op[0].r + 1] -= uso[0] * op[0].d;
	for (long long int i = 1; i < m; i++) {
		uso[i] += uso[i - 1];
		variacao[op[i].l] += uso[i] * op[i].d;
		if (op[i].r != n - 1) variacao[op[i].r + 1] -= uso[i] * op[i].d;
	}

	cout << a[0] + variacao[0];
	for (long long int i = 1; i < n; i++) {
		variacao[i] += variacao[i - 1];
		cout << " " << a[i] + variacao[i];
	}
	cout << endl;
	return 0;
}