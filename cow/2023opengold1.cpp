#include <iostream>
#include <vector>
#include <queue>
using namespace std;

bool solveOneTestCase() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    vector<int> C(N+1), S1(N+1), F1(N+1);
    vector<vector<int>> C_to_v(N+1);
    for(int i = 1; i <= N; i++) cin >> C[i];
    for(int i = 1; i <= N; i++) cin >> S1[i];
    for(int i = 1; i <= N; i++) cin >> F1[i];

    for(int i = 1; i <= N; i++){
        C_to_v[C[i]].push_back(i);
    }
    vector<vector<int>> adj(N+1);
    for(int i = 0; i < M; i++){
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    vector<bool> visited(N+1, false);
    auto weird = [&](vector<int> S, vector<int> F, bool is_S) -> bool {
        vector<bool> unlocked_colors(N+1, false);
        vector<bool> unlocked_verts(N+1, false);
        vector<bool> waiting(N+1, false);

        queue<int> q;
        q.push(1);
        while(!q.empty()){
            int v = q.front();
            q.pop();
            visited[v] = true;
            if(!unlocked_colors[S[v]]){
                unlocked_colors[S[v]] = true;
                for(int u: C_to_v[S[v]]){
                    if(!visited[u]){
                        unlocked_verts[u] = true;
                        if(waiting[u]){
                            q.push(u);
                        }
                    }
                }
            }
            for(int w: adj[v]){
                if((unlocked_verts[w]) || (!is_S && C[w] == S[w])){ // technically C=F but switched rn.
                    if(!visited[w]){
                        q.push(w);
                    }
                }
                else{
                    if(!visited[w]){
                        waiting[w] = true;
                    }
                }
            }
        }
        for(int i = 1; i <= N; i++){
            if(!visited[i]){
                if(S[i] != F[i]){
                    return false;
                }
            }
        }
        return true;
    };

    if(weird(S1, F1, true)){
        for(int i = 1; i <= N; i++){
            if(visited[i]){
                visited[i] = false;
            }
            else{
                visited[i] = true;
            }
        }
        if(weird(F1, S1, false)){
            return true;
        }
    }
    return false;
}

int main(){
    int T;
    cin >> T;
    while(T--){
        bool q = solveOneTestCase();
        if(q){
            cout << "YES\n";
        }
        else{
            cout << "NO\n";
        }
    }
    return 0;
}
