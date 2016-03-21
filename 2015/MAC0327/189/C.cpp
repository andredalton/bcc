#include <iostream>
#include <list>

using namespace std;

int main() {
    int n,
        i,
        j,
        pos = 0,
        mov = 0,
        a[200000],
        b[200000];
    cin >> n;

    for (int i=0; i<n; i++){
        cin >> a[i];
    }

    for (int i=0; i<n; i++){
        cin >> b[i];
    }

    pos = 0;
    for (i=0; i<n; i++) {
        for (j=pos; j<n; j++) {
            if (b[j] == a[i]) break;
        }
        pos = j;
        if (j==n) break;
    }

    cout << n-i << endl;

    return 0;
}
