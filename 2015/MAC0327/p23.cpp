#include <iostream>
#include <string>
#include <stack>
#include <list>
#include <vector>

using namespace std;

static const unsigned long int MAX_SIZE = 75000;

vector <list <int> > vec(MAX_SIZE);

int busca(string &amusing, string &last, int p, int w) {
    int i;

    for (i=p; i<last.size() && last[i]==amusing[i-p]; i++)
        if (last[i]==amusing[0]) vec[w].push_back(i);

    if ( i==last.size() ) return w;

    if ( last[i]==amusing[0] && i!=p) {
        int wu = busca(amusing, last, i, w+1);
        if ( wu ) return wu;
    }


    while ( vec[w].size()>1 ) {
        int pop = vec[w].back();
        vec[w].pop_back();
        int wu = busca(amusing, last, pop, w+1);
        if ( wu ) return wu;
    }

    return 0;
}

int main() {
    string amusing, last;
    int resp;

    getline( cin, amusing);
    getline( cin, last);

    resp = busca(amusing, last, 0, 0);

    if (resp) {
        cout << "NO" << endl;
        int l = 0;
        for (int i = 1; i <= resp; ++i) {
            int r = vec[i].front();
            cout << last.substr(l, r-l) << " ";
            l = r;
        }
        cout << last.substr(l) << endl;
    }
    else cout << "YES" << endl;

    return 0;
}
