#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;
 
typedef long long ll;
 
struct Bound {
    ll L, R;
};
 

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    int t;
    cin >> t;
    while(t--){
        int n;
        cin >> n;
        vector<Bound> bounds(n+1);
        for (int i = 1; i <= n; i++){
            cin >> bounds[i].L >> bounds[i].R;
        }
 
        vector<vector<int>> graph(n+1);
        for (int i = 1; i < n; i++){
            int u, v;
            cin >> u >> v;
            graph[u].push_back(v);
            graph[v].push_back(u);
        }
 
        // BFS from node 1
        vector<int> parent(n+1, 0), depth(n+1, 0);
        parent[1] = -1;
        depth[1] = 0;
        queue<int> qu;
        qu.push(1);
        while(!qu.empty()){
            int u = qu.front();
            qu.pop();
            for (int v : graph[u]){
                if(v == parent[u]) continue;
                parent[v] = u;
                depth[v] = depth[u] + 1;
                qu.push(v);
            }
        }
 
        vector<int> order;
        order.reserve(n);
        for (int i = 1; i <= n; i++){
            order.push_back(i);
        }
        sort(order.begin(), order.end(), [&](int a, int b){
            return depth[a] > depth[b];
        });
 
        vector<ll> chosen_val(n+1, 0);
        ll complement_sum = 0;
 
        for (int u : order){
            ll max_child = 0;
            for (int v : graph[u]){
                if(v == parent[u]) continue;
                max_child = max(max_child, chosen_val[v]);
            }
 
            ll L = bounds[u].L, R = bounds[u].R;
            ll chosen;
            if(max_child < L) chosen = L;
            else if(max_child > R) chosen = R;
            else chosen = max_child;
 
            for (int v : graph[u]){
                if(v == parent[u]) continue;
                complement_sum += max(0LL, chosen_val[v] - chosen);
            }
 
            chosen_val[u] = chosen;
        }
 
        ll ans = chosen_val[1] + complement_sum;
        cout << ans << "\n";
    }
    return 0;
}
