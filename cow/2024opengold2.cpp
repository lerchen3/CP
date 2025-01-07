#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace std;
using namespace __gnu_pbds;

template<typename T>
using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

struct Event {
    int sort_value;
    int queryval1, queryval2;
    int insertion;
    bool isinsertion;
    int index;
};

void solve() {
    int n;
    cin >> n;
    vector<int> l(n), r(n), k(n);
    
    for (int i = 0; i < n; i++) {
        cin >> l[i] >> r[i] >> k[i];
    }

    vector<Event> typeA;
    for (int i = 0; i < n; i++) {
        typeA.push_back({l[i], l[i] + k[i], r[i], 0, false, i});
        typeA.push_back({l[i], 0, 0, r[i], true, i});
    }

    vector<Event> typeB;
    for (int i = 0; i < n; i++) {
        typeB.push_back({k[i], l[i] + 1, r[i] - k[i], 0, false, i});
        typeB.push_back({r[i] - l[i], 0, 0, l[i], true, i});
    }

    sort(typeA.begin(), typeA.end(), [](const Event& a, const Event& b) {
        return a.sort_value == b.sort_value ? a.isinsertion : a.sort_value < b.sort_value;
    });

    sort(typeB.begin(), typeB.end(), [](const Event& a, const Event& b) {
        return a.sort_value == b.sort_value ? a.isinsertion : a.sort_value > b.sort_value;
    });

    vector<int> countA(n), countB(n);
    ordered_set<pair<int, int>> setA, setB;  

    int timestampA = 0;
    for (const Event& e : typeA) {
        if (e.isinsertion) {
            setA.insert({e.insertion, timestampA++});
        } else {
            countA[e.index] = setA.size() - setA.order_of_key({e.queryval1, -1});
        }
    }

    int timestampB = 0;
    for (const Event& e : typeB) {
        if (e.isinsertion) {
            setB.insert({e.insertion, timestampB++});
        } else {
            if (e.queryval2 >= e.queryval1) {
                countB[e.index] = setB.order_of_key({e.queryval2 + 1, -1}) - 
                                 setB.order_of_key({e.queryval1, -1});
            }
        }
    }

    for (int i = 0; i < n; i++) {
        cout << countA[i] + countB[i] - 1 << "\n";
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    solve();
    return 0;
}
