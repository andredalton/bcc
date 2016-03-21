#include<bits/stdc++.h>


using namespace std;

void printGraph(map<pair<char, int>, list<pair<char, int> > >& graph) {
  for (auto i: graph) {
    //list<pair<char, int> >::iterator it = graph[i].begin();

    //if (it != graph[i].end()) {
    //cout << i.second << " :" << endl;

      for (auto j: i)
      	cout << ' ' << j.second;

      cout << '\n';
    }
  }
}

int main() {
  int t, n, m;
  vector<int> A (101), B (101);
  map<pair<char, int>, list<pair<char, int> > > graph;

  cin >> t;

  for (int i = 0; i < t; i++) {
    cin >> n;

    for (int j = 1; j <= n; j++) {
      cin >> A[j];
    }

    cin >> m;

    for (int j = 1; j <= m; j++) {
      cin >> B[j];
    }

    for (int j = 1; j <= n; j++) {
      for (int k = 1; k <= m; k++) {
	if (B[k] % A[j] == 0) {
	  graph[make_pair('A', A[j])].push_back(make_pair('B', B[k]));
	  graph[make_pair('B', B[k])].push_back(make_pair('A', A[j]));
	}
      }
    }
    //printGraph(graph);
  }

  return 0;
}


//#include <iostream>
//#include <string>
//#include <map>
//#include <vector>
//#include <list>
//#include <set>
//
//using namespace std;
//
//typedef struct elemento   {
//    int e;
//    short int t;
//    set <int> conj;
//} elemento;
//
//int calcSoma(elemento E, vector<elemento> S) {
//    int s = 0;
//    for (auto it: E.conj) s += S[it].t;
//    return s;
//}
//
//int main() {
//    short int T, teste = 1;
//
//    cin >> T;
//    while (teste<T+1) {
//        int n , m, mem, total = 0;
//        cin >> n;
//        vector<elemento> A(n);
//        for (int i=0; i<n; i++) {
//            cin >> mem;
//            A[i].e = mem;
//        }
//
//        cin >> m;
//        vector<elemento> B(m);
//        for (int i=0; i<m; i++) {
//            cin >> mem;
//            int divs = 0;
//            B[i].e = mem;
//            for (int j=0; j<n; j++) {
//                if (A[j].e!=0 && mem%A[j].e==0) {
//                    total++;
//                    A[j].t++;
//                    B[i].t++;
//                    A[j].conj.insert(i);
//                    B[i].conj.insert(j);
//                }
//            }
//        }
//
//        // Apenas para visualizar a estrutura de dados.
//        for (int i=0; i<n; i++) {
//            cout << A[i].e << ": ";
//            for (auto it: A[i].conj) cout << B[it].e << " ";
//            cout << "\tL: "<< A[i].t << endl;
//        }
//
//        cout << endl << endl;
//
//        for (int i=0; i<m; i++) {
//            cout << B[i].e << ": ";
//            for (auto it: B[i].conj) cout << A[it].e << " ";
//            cout << "\tL: "<< B[i].t << endl;
//        }
//        /* */
//
//        int apagados = 0;
//        while(total>0) {
//            int index = -1;
//            int maximo = 0;
//            int maxSoma = 0;
//            for (int i=0; i<n; i++) {
//                if (maximo<A[i].t) {
//                    maximo = A[i].t;
//                    index = i;
//                    maxSoma = calcSoma(A[i], B);
//                }
//                else if (maximo==A[i].t) {
//                    int somai = calcSoma(A[i], B);
//                    if (somai > maxSoma) {
//                        index = i;
//                        maxSoma = somai;
//                    }
//                }
//            }
//            for (int i=0; i<m; i++) {
//                if (maximo<B[i].t) {
//                    maximo = B[i].t;
//                    index = n+i;
//                    maxSoma = calcSoma(B[i], A);
//                }
//                else if (maximo==B[i].t) {
//                    int somai = calcSoma(B[i], A);
//                    if (somai > maxSoma) {
//                        index = n+i;
//                        maxSoma = somai;
//                    }
//                }
//            }
//            if (index<n) {
//                cout << "Removendo " << A[index].e << "[" << index << "] de A. " << A[index].t << endl;
//                for (auto it: A[index].conj) {
////                    cout << B[it].e << " ";
//                    if (B[it].t>0) B[it].t--;
//                }
////                cout << endl;
//                total -= A[index].t;
//                A[index].t = 0;
//            }
//            else {
//                cout << "Removendo " << B[index-n].e << "[" << index-n << "] de B. " << B[index].t << endl;
//                for (auto it: B[index-n].conj) if (A[it].t>0) A[it].t--;
//                total -= B[index-n].t;
//                B[index-n].t = 0;
//            }
///*
//            for (int i=0; i<n; i++) {
//                cout << A[i].e << ": ";
//                for (auto it: A[i].conj) {
//                    cout << it+1 << " ";
//                }
//                cout << "\tL: "<< A[i].t << endl;
//            }
//
//            cout << endl << endl;
//
//            for (int i=0; i<m; i++) {
//                cout << B[i].e << ": ";
//                for (auto it: B[i].conj) {
//                    cout << it+1 << " ";
//                }
//                cout << "\tL: "<< B[i].t << endl;
//            }
//*/
//            apagados += 1;
//        }
//
//        cout << "Case " << teste << ": " << apagados << endl;
//        teste++;
//    }
//
//    return 0;
//}
