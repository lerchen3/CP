#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MOD = 1e9 + 7;

ll power_mod(ll base, ll exp) {
    ll res = 1;
    base %= MOD;
    while(exp > 0){
        if(exp & 1){
            res = (res * base) % MOD;
        }
        base = (base * base) % MOD;
        exp >>=1;
    }
    return res;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    freopen("help.in", "r", stdin);
    freopen("help.out", "w", stdout);
    
    int N;
    cin >> N;
    
    vector<pair<int, int>> intervals(N);
    
    unordered_map<int, int> left_to_right;
    
    vector<int> endpoints;
    
    for(int i=0;i<N;i++){
        cin >> intervals[i].first >> intervals[i].second;
        left_to_right[intervals[i].first] = intervals[i].second;
        endpoints.push_back(intervals[i].first);
        endpoints.push_back(intervals[i].second);
    }
    
    sort(endpoints.begin(), endpoints.end());
    
    unordered_map<int, bool> is_left;
    for(auto &[l, r] : intervals){
        is_left[l] = true;
        is_left[r] = false;
    }
    
    ll ans = 0;
    unordered_set<int> alive_right_endpoints;
    int num_left_endpoints_processed = 0;
    
    vector<ll> pow2(2*N +1, 1);
    for(int i=1;i<=2*N;i++){
        pow2[i] = (pow2[i-1] * 2) % MOD;
    }
    
    for(auto x : endpoints){
        if(is_left[x]){
            ans = (ans * 2) % MOD;            
            int exponent = num_left_endpoints_processed - alive_right_endpoints.size();
            if(exponent >=0){
                ans = (ans + pow2[exponent]) % MOD;
            }
            else{
                ans = (ans + 0) % MOD;
            }
            alive_right_endpoints.insert(left_to_right[x]);
            num_left_endpoints_processed +=1;
        }
        else{
            alive_right_endpoints.erase(x);
        }
    }
    
    cout << ans;
    
    return 0;
}
