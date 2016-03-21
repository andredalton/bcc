/*
Nome: André Meneghelli Vale
NUSP: 4898948
Problema P30 - B. Number of Triplets
*/

#include<bits/stdc++.h>

using namespace std;

class ponto {
public:
	int x, y;

	inline bool operator<( const ponto& P) const {
		return (x != P.x) ? (x < P.x) : (y < P.y);
	}

	bool valido(const ponto P) const {
	    return (((x + P.x) % 2 == 0) && ((y + P.y) % 2 == 0));
	}

	ponto merge(const ponto P) const {
        ponto pt;
        pt.x = (x + P.x) / 2;
        pt.y = (y + P.y) / 2;
        return pt;
	}
};

const int MAXN = 3001;

int main()
{
	int n;
	cin >> n;
	set<ponto> pontos;

	for (int i = 0; i < n; ++i) {
		ponto p;
		cin >> p.x >> p.y;
		pontos.insert( p);
	}
	int count = 0;
	for (set<ponto>::iterator it1 = pontos.begin(); it1 != pontos.end(); it1++) {
		for (set<ponto>::iterator it2 = it1; it2 != pontos.end(); it2++) {
		    if (it1==it2) continue;
			if (it1->valido(*it2)) {
				ponto p = it1->merge(*it2);
				if (pontos.find( p) != pontos.end()) {
					count++;
				}
			}
		}
	}
	cout << count << endl;

	return 0;
}