#include <iostream>
#include <cstring>
#include <cstdlib>
#include <list>
#include <vector>
#include <set>

using namespace std;

static const unsigned long int MAX_SIZE = 1000000;

class Node {
    public:
        int index;
        int nparents;
        int nchildren;
        vector <Node> children;
        void newNode(int index) {
            this->index = index;
            this->nparents = 0;
            this->nchildren = 0;
            this->children.clear();
        }
        void addChild(int c, Node psticks[], set <int> &zeros) {
            cout << "Add " << c+1 << " in " << this->index+1 << endl;
            if (this->nparents==0) {
                cout << "Add " << this->index+1 << " in zeros." << endl;
                zeros.insert(this->index);
            }
            this->nchildren++;
            this->children.push_back(psticks[c]);
            psticks[c].nparents++;
            cout << psticks[c].index+1 << " is adopted again, " << psticks[c].nparents << endl;
            zeros.erase(c);
        }
        void take(set <int> &zeros, list <int> &out) {
            cout << "Take " << this->index+1 << endl;
            zeros.erase(this->index);
            out.push_back(this->index);
            for (int i=0; i<this->nchildren; i++) {
                cout << this->children[i].index+1 << " has " << this->children[i].nparents << " parents." << endl;
                this->children[i].nparents--;
                cout << this->children[i].index+1 << " lost a parent, remain " << this->children[i].nparents << endl;
                if (this->children[i].nparents==0) {
                    cout << this->children[i].index+1 << " is orphaned." << endl;
                    zeros.insert(i);
                }
            }
        }
};

Node psticks[MAX_SIZE];

int main() {
    int m, n;

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

        for (int i=0; i<n; i++) {
            cout << psticks[i].index << endl;
            cout << psticks[i].nchildren << endl;
            cout << psticks[i].nparents << endl;
        }

        while (!zeros.empty())
            psticks[0].take(zeros, out);

        cout << zeros.size() << endl;
        cout << out.size() << endl;
    }

    return 0;
}
