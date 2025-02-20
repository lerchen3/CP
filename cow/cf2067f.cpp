#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
using namespace std;
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    const long long MOD = 1000000007;
    
    while(t--){
        int n;
        cin >> n;
        vector<int> a(n);
        for (int i = 0; i < n; i++){
            cin >> a[i];
        }
        
        vector<int> S(n+1, 0);
        S[0] = 0;
        for (int i = 0; i < n; i++){
            S[i+1] = S[i] ^ a[i];
        }
        
        vector<int> uniq = S;
        sort(uniq.begin(), uniq.end());
        uniq.erase(unique(uniq.begin(), uniq.end()), uniq.end());
        
        unordered_map<int, int> mp;
        mp.reserve(uniq.size() * 2);
        for (int i = 0; i < (int)uniq.size(); i++){
            mp[uniq[i]] = i;
        }
        
        int m = uniq.size();
        vector<long long> dp(m, 0);
        
        dp[mp[0]] = 1; 
        
        for (int k = 1; k <= n; k++){
            int id_prev = mp[S[k-1]];
            int id_curr = mp[S[k]];
            dp[id_prev] = (dp[id_prev] * 3LL) % MOD;
            dp[id_prev] = (dp[id_prev] + 2LL * dp[id_curr]) % MOD;
        }
        long long ans = 0;
        for (auto v : dp){
            ans = (ans + v) % MOD;
        }
        cout << ans << "\n";
    }
    return 0;
}
