#include<iostream>
#include<vector>

class operacao {
public:
	int op, l, r, v;
};

using namespace std;

int main() {
    int n, m;
    vector<operacao> operacoes(5005);
    vector<long> array(5005, 1e9), diferenca(5005, 0);
    int direita;

    cin >> n >> m;

    for (int i = 0; i < m; i++) {
        operacao nova;
        cin >> nova.op >> nova.l >> nova.r >> nova.v;
        operacoes[i] = nova;

        if (nova.op == 1)
            for (int j = (nova.l)-1; j < nova.r; j++) diferenca[j] += nova.v;
        else {
            direita = 0;
            for (int j = (nova.l)-1; j < nova.r; j++)
            if (array[j] >= nova.v - diferenca[j]) {
                array[j] = nova.v - diferenca[j];
                direita = 1;
            }
            if (!direita) {
                cout << "NO" << endl;
                return 0;
            }
        }
    }

    vector<long> test(array);

    for (int i = 0; i < m; i++) {
        if (operacoes[i].op == 1) {
            for (int j = (operacoes[i].l)-1; j < operacoes[i].r; j++) test[j] += operacoes[i].v;
        }
        else {
            direita = 0;
            for (int j = (operacoes[i].l)-1; j < operacoes[i].r; j++) {
                if (test[j] > operacoes[i].v) {
                    cout << "NO" << endl;
                    return 0;
                }
                else if (test[j] == operacoes[i].v) direita = 1;
            }
            if (!direita) {
                cout << "NO" << endl;
                return 0;
            }
        }
    }

    cout << "YES" << endl;
    for (int i = 0; i < n; i++) cout << array[i] << " ";

    return 0;
}