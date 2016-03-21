/*
Nome: André Meneghelli Vale
NUSP: 4898948
Problema P27 - D. Two Paths
*/

#include<bits/stdc++.h>

using namespace std;

static int lbl[202];

class Grafo {
public:
	unsigned int V;
	unsigned int A;
	list<long long int> *vizinho;

	Grafo( unsigned int v){
		V = v;
		A = 0;
		vizinho = new list<long long int>[v];
	}

	void adiciona( long long int v, long long int w) {
		vizinho[v].push_back( w);
		vizinho[w].push_back( v);
	}

	void remove( long long int v, long long int w) {
		vizinho[v].remove( w);
		vizinho[w].remove( v);
	}

	int64_t maiorCaminho( long long int v) {
		for (long long int w = 0; w < V; ++w) lbl[w] = 0;
		list<long long int> livres;
		verticesLivres( livres, v, v);
		int64_t maxs = 0;
		for (auto f : livres) {
			for (long long int w = 0; w < V; ++w) lbl[w] = 0;
			int64_t ms = procuraMenorCaminho( f, f);
			if (ms > maxs) maxs = ms;
		}
		return maxs;
	}

	void verticesLivres( list<long long int>& livres, long long int v, long long int ini) {
		if (v != ini && vizinho[v].size() <= 1) {
			livres.push_back( v);
		}
		else {
            lbl[v] = 1;
            for (auto w : vizinho[v]) if (lbl[w] == 0) verticesLivres( livres, w, ini);
            lbl[v] = 0;
		}
		return;
	}

	int64_t procuraMenorCaminho( long long int v, long long int ini) {
		int64_t maxv = -1;
		if (v != ini && vizinho[v].size() <= 1) return 0;
		lbl[v] = 1;
		for (auto w : vizinho[v]) {
			if (lbl[w] == 0) {
				int64_t maxw = procuraMenorCaminho( w, ini);
				if (maxw != -1 && maxv < 1 + maxw) maxv = 1 + maxw;
			}
		}
		lbl[v] = 0;
		return maxv;
	}
};


int main() {
	int n;
	cin >> n;

	Grafo Cidades(n);
	list<pair<long long int, long long int> > estradas;

	for (int i = 0; i < n - 1; i++) {
		long long int v, w;
		cin >> v >> w;
		v--;
		w--;
		Cidades.adiciona( v, w);
		estradas.push_back( make_pair( v, w));
	}
	int64_t maxs = 0;
	for (auto e : estradas) {
		Cidades.remove( e.first, e.second);
		int64_t sm = Cidades.maiorCaminho( e.first);
		int64_t sm2 = Cidades.maiorCaminho( e.second);
		Cidades.adiciona( e.first, e.second);
		sm *= sm2;
		if (sm > maxs) {
			maxs = sm;
		}
	}
	cout << maxs << endl;

	return 0;
}