#include <vector>
#include <set>
#include <iostream>
using namespace std;

bool isMagical(const vector<long long>& seq) {
    int n = seq.size();
    if (n <= 1) return true;
    
    vector<long long> minPrefix(n);
    minPrefix[0] = seq[0];
    for (int i = 1; i < n; i++) minPrefix[i] = min(minPrefix[i-1], seq[i]);
    
    vector<long long> mexSuffix(n);
    set<long long> missing;
    for (long long i = 0; i <= n; i++) missing.insert(i);
    
    for (int i = n-1; i >= 0; i--) {
        if (seq[i] <= n) missing.erase(seq[i]);
        mexSuffix[i] = *missing.begin();
    }
    
    for (int i = 0; i < n-1; i++) {
        if (minPrefix[i] < mexSuffix[i+1]) return false;
    }
    return true;
}

void solve() {
    int n;
    cin >> n;
    vector<long long> a;
    a.reserve(n);
    long long x;
    bool hasZero = false;
    for (int i = 0; i < n; i++) {
        cin >> x;
        a.push_back(x);
        if (x == 0) hasZero = true;
    }
    
    if (!hasZero) {
        cout << n << '\n';
        return;
    }
    
    vector<long long> b;
    b.reserve(n);
    bool firstZeroTaken = false;
    for (const long long& x : a) {
        if (x != 0) b.push_back(x);
        else if (!firstZeroTaken) {
            b.push_back(x);
            firstZeroTaken = true;
        }
    }
    
    if (isMagical(b)) cout << b.size() << '\n';
    else {
        int nonZeroCount = n;
        for (const long long& x : a) if (x == 0) nonZeroCount--;
        cout << nonZeroCount << '\n';
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) solve();
    return 0;
}
