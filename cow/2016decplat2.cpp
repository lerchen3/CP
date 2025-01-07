#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MOD = 1000000009;

vector<int> read_scores(int count) {
    vector<int> scores(count);
    for(int &x : scores) cin >> x;
    return scores;
}

int main(){
    freopen("team.in", "r", stdin);
    freopen("team.out", "w", stdout);
    
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    int N, M, K;
    cin >> N >> M >> K;
    
    vector<int> FJ = read_scores(N);
    vector<int> FP = read_scores(M);
    
    sort(FJ.begin(), FJ.end());
    sort(FP.begin(), FP.end());
    
    vector<vector<ll>> dp_prev(K+1, vector<ll>(M+1, 0));
    vector<vector<ll>> dp_curr(K+1, vector<ll>(M+1, 0));
    
    for(int m=0; m<=M; m++) {
        dp_prev[0][m] = 1;
    }
    
    for(int n=1; n<=N; n++){
        for(int k=0; k<=K; k++) {
            for(int m=0; m<=M; m++) {
                dp_curr[k][m] = 0;
            }
        }
        for(int m=0; m<=M; m++){
            for(int k=0; k<=K; k++){
                ll val = dp_prev[k][m];
                
                if(m >0){
                    val += dp_curr[k][m-1];
                    if(val >= MOD) val -= MOD;
                }
                
                if(n >0 && m >0){
                    val -= dp_prev[k][m-1];
                    if(val <0) val += MOD;
                }
                
                if(n >0 && m >0 && FJ[n-1] > FP[m-1] && k >0){
                    val += dp_prev[k-1][m-1];
                    if(val >= MOD) val -= MOD;
                }
                
                dp_curr[k][m] = val;
            }
        }
        dp_prev.swap(dp_curr);
    }
    
    ll answer = dp_prev[K][M];
    cout << answer;
}
