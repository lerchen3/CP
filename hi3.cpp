#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <set>

using namespace std;
 
int popcount(int x) {
    int count = 0;
    while (x) {
        count += x & 1;
        x >>= 1;
    }
    return count;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int N, M;
    cin >> N >> M;
    vector<int> adj(N, 0);
    vector<vector<bool>> hasEdge(N, vector<bool>(N,false));
    int te = 0;
    for (int i = 0; i < M; i++){
        int a, b;
        cin >> a >> b;
        a--; b--;
        if(!hasEdge[a][b]){
            hasEdge[a][b] = hasEdge[b][a] = true;
            adj[a] |= (1 << b);
            adj[b] |= (1 << a);
            te++;
        }
    }
    
    int sz = 1 << N;
    vector<int> inEdges(sz, 0);
    for (int mask = 0; mask < sz; mask++){
        int count = 0;
        for (int i = 0; i < N; i++){
            if(mask & (1 << i)){
                for (int j = i + 1; j < N; j++){
                    if(mask & (1 << j)){
                        if(hasEdge[i][j])
                            count++;
                    }
                }
            }
        }
        inEdges[mask] = count;
    }
 
    vector<int> f(sz, 0);
    for (int mask = 1; mask < sz; mask++){
        int cnt = popcount(mask);
        int ideal = cnt*(cnt-1)/2;
        f[mask] = ideal - 2 * inEdges[mask];
    }
    f[0] = 0;
     
    vector<int> dp(sz, -1000000000);
    dp[0] = 0;
    for (int mask = 1; mask < sz; mask++){
        for (int s = mask; s; s = (s - 1) & mask)
            dp[mask] = max(dp[mask], dp[mask ^ s] + f[s]);
    }
     
    int best = dp[sz - 1];
    int tp = N*(N-1)/2;
    int answer = (tp - te) - best;
    cout << answer << "\n";
    return 0;
}
