#include <iostream>
#include <cstring>
#include <cstdlib>
#include <list>
#include <vector>
#include <map>


using namespace std;

static const unsigned int MAX_SIZE = 5000;

void dict() {
    map<char, map<char>> v =
}

int main() {
    string word



    cin >> n >> m;

    while (n!=0) {
        set <int> zeros;
        list <int> out;
        for (int i=0; i<n; i++) {
            psticks[i].newNode(i);
        }
        for (int i=0; i<m; i++) {
            int s1, s2;
            cin >> s1 >> s2;
            if (s2==0) {
                n = 0;
                break;
            }
            psticks[s1-1].addChild(s2-1, psticks, zeros);
        }
        while (!zeros.empty())
            psticks[0].take(zeros, out);

        cout << zeros.size() << endl;
        cout << out.size() << endl;
    }

    return 0;
}
