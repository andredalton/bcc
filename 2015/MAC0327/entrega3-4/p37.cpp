/*
Nome: André Meneghelli Vale
NUSP: 4898948
Problema P37 - B. Cow Program
*/

#include<bits/stdc++.h>

using namespace std;

static const long long int MAX = 2e5 + 1;

#define INCREMENTA 1
#define DECREMENTA 0

typedef struct node {
	long long int v;
	long long int custo;
	bool visitado;
} node;

class Grafo {
public:
	long long int n;
	list<node> vizinhos[MAX][2];
	Grafo( long long int N){
	    n = N;
	};

	vector<long long int> buscaLargura() {
        vector<long long int> sum( n + 1, -1);
        long long int v = 0;
        queue<pair<long long int, list<node>::iterator> > pilha[2];
        for (list<node>::iterator it = vizinhos[v][DECREMENTA].begin(); it != vizinhos[v][DECREMENTA].end(); it++)
            if (!it->visitado) pilha[DECREMENTA].push( make_pair( v, it));
        for (list<node>::iterator it = vizinhos[v][INCREMENTA].begin(); it != vizinhos[v][INCREMENTA].end(); it++)
            if (!it->visitado) pilha[INCREMENTA].push( make_pair( v, it));

        bool estado = DECREMENTA;
        while (!pilha[estado].empty() || !pilha[!estado].empty()) {
            if (pilha[estado].empty()) estado = !estado;
            v = pilha[estado].front().first;
            list<node>::iterator itw = pilha[estado].front().second;
            pilha[estado].pop();
            itw->visitado = true;
            for (list<node>::iterator it = vizinhos[itw->v][!estado].begin(); it != vizinhos[itw->v][!estado].end(); it++) {
                if (!it->visitado) {
                    it->custo += itw->custo;
                    if (it->v == 1) sum[itw->v] = it->custo;
                    else pilha[!estado].push( make_pair( itw->v, it));
                }
            }
            estado = !estado;
        }
        return sum;
    }
};

int main()
{
	long long int n;
	cin >> n;
	Grafo graph( n);

	for (long long int i = 2; i <= n; i++) {
		long long int a, c;
		cin >> a;
		node v = { 1, i - 1, false };
		graph.vizinhos[i][INCREMENTA].push_back( v);
		v = { i, a, 0 };
		c = i + a;
		if (c > n) graph.vizinhos[0][INCREMENTA].push_back( v);
		else graph.vizinhos[c][INCREMENTA].push_back( v);
		c = i - a;
		if (c <= 0) graph.vizinhos[0][DECREMENTA].push_back( v);
		else graph.vizinhos[c][DECREMENTA].push_back( v);
	}

	vector<long long int> sum = graph.buscaLargura();
	for (long long int i = 2; i <= n; i++) cout << sum[i] << endl;

	return 0;
}