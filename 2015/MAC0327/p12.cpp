#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <list>
#include <set>
#include <vector>

using namespace std;

static const unsigned long int MAX_SIZE = 10000;
static const unsigned long int INITIAL_NUMBER_OF_BUS = 1;

int main() {
    unsigned short **mtz;

    mtz = (unsigned short **) malloc(MAX_SIZE*sizeof(short*));
    for (short i=0; i<MAX_SIZE; i++) {
        mtz[i] = (unsigned short *) malloc(MAX_SIZE*sizeof(short));
        memset(mtz[i], 0, MAX_SIZE*sizeof(short));
    }

    int n;
    unsigned short maxid = 0;
    cin >> n;

    vector< set<unsigned short> > ins(INITIAL_NUMBER_OF_BUS);
    vector< set<unsigned short> > outs(INITIAL_NUMBER_OF_BUS);

    for (unsigned short i=1; i<=n; i++) {
        unsigned short m, a, b;
        cin >> m >> a;
        for (int j=0; j<m; j++) {
            cin >> b;
            if (b>maxid)
                maxid = b;
            while (maxid - 1 >= outs.size()) {
                ins.resize(ins.size() * 2);
                outs.resize(outs.size() * 2);
            }
            outs[a - 1].insert(i);
            ins[b - 1].insert(i);
            mtz[a-1][b-1] = i;
            a = b;
        }
    }

    cout << maxid << endl;

    for (int i=0; i<maxid; i++) {
        for (int j=0; j<maxid; j++) {
            cout << mtz[i][j] << " ";
        }
        cout << endl;
    }

    cout << endl;

    for (auto& it : outs) {
        for (auto& it2 : it) {
            cout << it2 << " ";
        }
        cout << endl;
    }

    cout << endl;

    for (auto& it : ins) {
        for (auto& it2 : it) {
            cout << it2 << " ";
        }
        cout << endl;
    }



    return 0;
}
