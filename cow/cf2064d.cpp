#include <iostream>
#include <vector>
using namespace std;
using ll = long long;

void solve() {
    int n, q;
    cin >> n >> q;
    vector<ll> w(n);
    for (int i = 0; i < n; i++) {
        cin >> w[i];
    }
    
    vector<ll> suffix_xor(n + 1, 0);
    for (int i = n-1; i >= 0; i--) {
        suffix_xor[i] = suffix_xor[i+1] ^ w[i];
    }
    
    vector<vector<int>> power_indices(31); // length of bitstring.
    for (int bit = 0; bit < 30; bit++) {
        ll threshold = 1LL << bit;
        for (int i = 0; i < n; i++) {
            if (w[i] >= threshold) {
                power_indices[bit+1].push_back(i);
            }
        }
    }
    int ans = 0;
    // process queries
    while(q--) {
        int x;
        cin >> x;
        int original_x = x;
        int curindex = n;
        while(true){
            // find length of x in binary
            int length = 0;
            x = original_x ^ suffix_xor[curindex];
            if(x == 0){
                break;
            }
            int temp = x;
            while(temp > 0){
                length++;
                temp >>= 1;
            }

            // find largest index of power_indices[length] that is less than curindex.
            auto it = lower_bound(power_indices[length].begin(), power_indices[length].end(), curindex);
            if(it == power_indices[length].begin()){
                curindex = 0;
                break;
            }
            int idx = *(it-1);
            x = original_x ^ suffix_xor[idx+1];
            if(x < w[idx]){
                curindex = idx + 1;
                break;
            }
            curindex = idx;
        }
        cout << n - curindex << endl;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}