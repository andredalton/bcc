#include <iostream>
#include <algorithm>
#include <vector>
#include <array>

#include <cstring>
#include <cstdlib>
#include <list>
#include <set>


using namespace std;

bool deep_cmp(array<int, 4> t1, array<int, 4> t2) {
    return t1[0] < t2[0];
}

bool index_cmp(array<int, 4> t1, array<int, 4> t2) {
    return t1[2] < t2[2];
}

int main() {
    int t=-1, w, n, d, g;
    vector<array<int, 4>> ocean(30);

    while (!cin.eof()) {
        if (t>=0)
            cout << g << endl << d << endl << endl;
        cin >> t >> w >> n;
        d = g = 0;
        for (int i=0; i<n; i++) {
            cin >> ocean[i][0] >> ocean[i][1];
            ocean[i][2] = i;
            ocean[i][3] = 1;
        }

        cout << endl << "Inicial" << endl;
        for (int i=0; i<n; i++) {
            cout << ocean[i][0] << " " << ocean[i][1] << " " << ocean[i][2] << " " << ocean[i][3] << endl;
        }

        cout << endl << "Profundidade" << endl;
        sort (ocean.begin(), ocean.end());
        for (int i=0; i<n; i++) {
            cout << ocean[i][0] << " " << ocean[i][1] << " " << ocean[i][2] << " " << ocean[i][3] << endl;
        }

    }

    return 0;
}
