#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <utility>
#include <functional>
using namespace std;
 
using ll = long long;
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int N;
    cin >> N;
    vector<int> f(N+1), cost(N+1);
    for (int i = 1; i <= N; i++){
        cin >> f[i];
    }
    for (int i = 1; i <= N; i++){
        cin >> cost[i];
    }
 
    vector<int> state(N+1, 0);
    vector<bool> inCycle(N+1, false);
    vector<vector<int>> cycles;
    vector<int> cycleSize(N+1, 0);
 
    for (int i = 1; i <= N; i++){
        if(state[i] != 0) continue;
        vector<int> path;
        int cur = i;
        while (true) {
            if(state[cur] == 0){
                state[cur] = i;
                path.push_back(cur);
                cur = f[cur];
            } else {
                if(state[cur] == i){
                    vector<int> cycle;
                    int pos = 0;
                    while(path[pos] != cur) pos++;
                    for (int j = pos; j < (int)path.size(); j++){
                        cycle.push_back(path[j]);
                        inCycle[path[j]] = true;
                    }
                    cycles.push_back(cycle);
                    for (int node : cycle)
                        cycleSize[node] = cycle.size();
                }
                break;
            }
        }
        for (int node : path) {
            state[node] = -1;
        }
    }
 
    vector<vector<int>> children(N+1);
    for (int i = 1; i <= N; i++){
        if(f[i] == i) continue;
        if(inCycle[i] && inCycle[f[i]]) continue;
        children[f[i]].push_back(i);
    }
 
    vector<ll> dp_keep(N+1, 0), dp_change(N+1, 0);
    vector<bool> visited(N+1, false);
 
    function<void(int)> tree_helper = [&](int node) {
        visited[node] = true;
        for (int child : children[node]){
            if (!visited[child])
                tree_helper(child);
        }
        ll sum_dp_change = 0, sum_min = 0;
        for (int child : children[node]){
            sum_dp_change += dp_change[child];
            sum_min += min(dp_keep[child], dp_change[child]);
        }
        if(inCycle[node]){
            if(cycleSize[node] == 1){
                dp_keep[node]   = sum_dp_change;
                dp_change[node] = cost[node] + sum_min;
            } else {
                dp_keep[node]   = sum_dp_change;
                dp_change[node] = sum_min;
            }
        } else {
            dp_keep[node]   = sum_dp_change;
            dp_change[node] = cost[node] + sum_min;
        }
    };
 
    for (int i = 1; i <= N; i++){
        if (!visited[i])
            tree_helper(i);
    }
 
    ll ans = 0;
    const ll INF = 1e18;
 
    for (auto &cycle : cycles) {
        if(cycle.size() == 1){
            int node = cycle[0];
            ans += dp_change[node] - cost[node];
        } else {
            auto keep_val = [&](int node) -> ll { return dp_keep[node]; };
            auto change_val = [&](int node) -> ll { return dp_change[node] + cost[node]; };
 
            int k = cycle.size();
            vector<vector<ll>> dp(k, vector<ll>(2, INF));
            dp[0][1] = change_val(cycle[0]);
            dp[0][0] = INF;
 
            for (int i = 1; i < k; i++){
                int node = cycle[i];
                dp[i][0] = dp[i-1][1] + keep_val(node);
                dp[i][1] = min(dp[i-1][0], dp[i-1][1]) + change_val(node);
            }
            ll candidateA = min(dp[k-1][0], dp[k-1][1]);
 
            vector<vector<ll>> dp2(k, vector<ll>(2, INF));
            dp2[0][0] = keep_val(cycle[0]);
            dp2[0][1] = INF;
 
            for (int i = 1; i < k; i++){
                int node = cycle[i];
                dp2[i][0] = dp2[i-1][1] + keep_val(node);
                dp2[i][1] = min(dp2[i-1][0], dp2[i-1][1]) + change_val(node);
            }
            ll candidateB = dp2[k-1][1];
 
            ans += min(candidateA, candidateB);
        }
    }
 
    cout << ans << "\n";
    return 0;
}