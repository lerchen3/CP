#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MOD = 1000000007;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    freopen("spainting.in", "r", stdin);
    freopen("spainting.out", "w", stdout);
    
    ll N, M, K;
    cin >> N >> M >> K;
    
    if(K == 0){
        cout << 0;
        return 0;
    }
    if(K == 1){
        cout << 0;
        return 0;
    }
    
    vector<ll> dp_prev(K, 0);
    dp_prev[0] = M;
    
    for(ll i = 2; i <= N; i++){
        vector<ll> dp_current(K, 0);
        
        ll total_prev = 0;
        for(int j = 0; j < K; j++){
            total_prev = (total_prev + dp_prev[j]) % MOD;
        }
        
        dp_current[0] = (total_prev * (M - 1)) % MOD;
        
        for(int j = 1; j < K; j++){
            dp_current[j] = dp_prev[j - 1] % MOD;
        }
        
        dp_prev = dp_current;
    }
    
    ll invalid_sequences = 0;
    for(int j = 0; j < K; j++){
        invalid_sequences = (invalid_sequences + dp_prev[j]) % MOD;
    }
    
    ll total_sequences = 1;
    ll base = M;
    ll exponent = N;
    while(exponent > 0){
        if(exponent & 1){
            total_sequences = (total_sequences * base) % MOD;
        }
        base = (base * base) % MOD;
        exponent >>= 1;
    }
    
    ll valid_paintings = (total_sequences - invalid_sequences) % MOD;
    if(valid_paintings < 0){
        valid_paintings += MOD;
    }
    
    cout << valid_paintings;
    
    return 0;
}
