#include <iostream>
#include <cstdio>
#include <algorithm>
using namespace std;

bool myfunction (int i,int j) { return (i<j); }

void insert_in(long &v, int n, long value) {
    for (int i=0; i<n-1; i++)
        v[i] = v[i+1];
    v[n] = value;
}

int main() {
    int n, i;
    cin>>n;
    long *v = new long[n/2+1];

    for (i=0; i<n/2+1; i++) {
        v[i]=(long) i;
    }

    for (i=0; i<n/2+1; i++)
        cout << v[i] << endl;

/*
    for (i=0; i<n/2+1; i++) {
        cin>>v[i];
    }
/*
    std::sort(v, v+n);

    if (n%2==0) {
        printf("%.1lf", ((double)v[n/2-1] + (double)v[n/2])/2);
    }
    else {
        cout << v[n/2] << '\n';
    }
*/
    return 0;
}
