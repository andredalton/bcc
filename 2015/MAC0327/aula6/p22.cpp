#include <iostream>
#include <cstdio>
#include <algorithm>
using namespace std;

int main() {
    int n, i;
    cin>>n;
    long *v = new long[n];

    for (i=0; i<n; i++) {
        cin>>v[i];
    }

    std::sort(v, v+n);

    if (n%2==0) {
        printf("%.1lf", ((double)v[n/2-1] + (double)v[n/2])/2);
    }
    else {
        cout << v[n/2] << '\n';
    }

    return 0;
}
