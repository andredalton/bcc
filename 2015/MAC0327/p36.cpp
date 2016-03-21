#include<bits/stdc++.h>

using namespace std;

const int64_t MAX = 2e5 + 2;

int main() {
	int64_t n;
	cin >> n;
	int64_t dif[MAX];
	memset( dif, 0, MAX * sizeof (int64_t));
	int64_t ndif = 1;
	int64_t sum = 0;
	for (int64_t i = 0; i < n; i++) {
		int op;
		cin >> op;
		if (op == 1) {
			int64_t m, val;
			cin >> m >> val;
			if (m < ndif) {
				dif[m] -= val;
			}
			else {
				dif[m] += val;
			}
			sum += m * val;
		}
		else if (op == 2) {
			int64_t val;
			cin >> val;
			dif[ndif] = val - dif[ndif];
			dif[++ndif] = val;
			sum += val;
		}
		else {
			sum -= dif[ndif];
			dif[ndif - 1] -= dif[ndif];
			dif[ndif - 1] *= -1;
			--ndif;
		}
		printf( "%.6f\n", (double) sum / ndif);
	}
	return 0;
}