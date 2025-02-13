#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct DSU {
    long long n;
    vector<long long> parent, sz;
    long long totalPairs;

    DSU(long long n) : n(n), parent(n+1), sz(n+1,1), totalPairs(0) {
        for(long long i=1; i<=n; i++){
            parent[i] = i;
        }
    }
    long long findp(long long v){
        if(parent[v] == v) return v;
        return parent[v] = findp(parent[v]);
    }
    void unite(long long a, long long b){
        a = findp(a);
        b = findp(b);
        if(a == b) return;
        if(sz[a] < sz[b]) std::swap(a,b);
        totalPairs += (long long)sz[a]*sz[b]; 
        parent[b] = a;
        sz[a] += sz[b];
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long N, M;
    cin >> N >> M;

    string s;
    cin >> s;
    s = string(1, '#') + s;
    vector<vector<long long>> bigger(N+1);
    for(long long i=0; i<M; i++){
        long long u,v; 
        cin >> u >> v;
        if(u < v){
            bigger[u].push_back(v);
        } else {
            bigger[v].push_back(u);
        }
    }
    for(long long i=1; i<=N; i++){
        sort(bigger[i].begin(), bigger[i].end());
    }

    vector<vector<long long>> smaller(N+1);
    for(long long u=1; u<=N; u++){
        for(long long v: bigger[u]){
            smaller[v].push_back(u);
        }
    }

    DSU dsu(N);
    vector<vector<long long>> adj(N+1);

    for(long long u=1; u<=N; u++) {
        for(long long v: bigger[u]) {
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
    }

    for(long long v=1; v<=N; v++) {
        if(s[v] == '1') {
            long long max_neighbor = -1;
            for(long long u: adj[v]) {
                max_neighbor = max(max_neighbor, u);
            }
            
            if(max_neighbor > -1) {
                for(long long u: adj[v]) {
                    if(u != max_neighbor) {
                        adj[u].push_back(max_neighbor);
                        adj[max_neighbor].push_back(u);
                    }
                }
            }
        }
    }

    for(long long i=1; i<=N; i++) {
        sort(adj[i].begin(), adj[i].end());
        adj[i].erase(unique(adj[i].begin(), adj[i].end()), adj[i].end());
    }

    vector<long long> ans(N+1, 0LL);
    vector<bool> removed(N+1, true);

    for(long long t=N; t>=1; t--) {
        removed[t] = false;
        for(long long v: adj[t]) {
            if(!removed[v]) {
                dsu.unite(t, v);
            }
        }
        ans[t] = dsu.totalPairs;
    }

    for(long long i=1; i<=N; i++) {
        cout << ans[i] << "\n";
    }

    return 0;
}
