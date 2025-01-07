#include <bits/stdc++.h>
using namespace std;

const int DIRS = 4;
const int DR[DIRS] = {0, 0, -1, 1};
const int DC[DIRS] = {-1, 1, 0, 0};
const char DIR_CHARS[DIRS] = {'L', 'R', 'U', 'D'};

int dirCharToIndex(char c) {
    switch(c) {
        case 'L': return 0;
        case 'R': return 1;
        case 'U': return 2;
        case 'D': return 3;
        default: return -1;
    }
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, Q;
    cin >> N >> Q;
    
    struct Query {
        int r, c;
        char dir;
    };
    vector<Query> queries(Q);
    for(int i=0; i<Q; ++i){
        cin >> queries[i].r >> queries[i].c >> queries[i].dir;
        queries[i].r--; 
        queries[i].c--;
    }
    
    auto indexOf = [&](int r, int c) -> int {
        return r * N + c;
    };
    
    vector<char> grid(N*N, '?'); 
    for(int i=0; i<Q; ++i){
        int r = queries[i].r;
        int c = queries[i].c;
        char d = queries[i].dir;
        grid[indexOf(r, c)] = d;
    }
    
    vector<vector<int>> forward_adj(N*N + 1, vector<int>());
    for(int r=0; r<N; ++r) {
        for(int c=0; c<N; ++c) {
            int idx = indexOf(r, c);
            char d = grid[idx];
            
            if(d == '?') {
                bool added_outside = false;  
                for(int dir=0; dir<DIRS; ++dir) {
                    int nr = r + DR[dir];
                    int nc = c + DC[dir];
                    if(nr >= 0 && nr < N && nc >= 0 && nc < N) {
                        forward_adj[idx].push_back(indexOf(nr, nc));
                    } else if (!added_outside) {
                        forward_adj[idx].push_back(N*N);  
                        added_outside = true;
                    }
                }
            } else {
                int dir = dirCharToIndex(d);
                int nr = r + DR[dir];
                int nc = c + DC[dir];
                if(nr >= 0 && nr < N && nc >= 0 && nc < N) {
                    forward_adj[idx].push_back(indexOf(nr, nc));
                } else {
                    forward_adj[idx].push_back(N*N);
                }
            }
        }
    }
    
    vector<vector<int>> adj(N*N + 1, vector<int>());
    for(int u = 0; u <= N*N; ++u) {
        for(int v : forward_adj[u]) {
            adj[v].push_back(u);
        }
    }
    
    vector<bool> good(N*N + 1, false);
    queue<int> q;
    q.push(N*N);
    good[N*N] = true;
    while(!q.empty()){
        int u = q.front(); q.pop();
        for(auto &v : adj[u]){
            if(!good[v]){
                good[v] = true;
                q.push(v);
            }
        }
    }
    
    long long goodCount = 0;
    for(int i=0; i<N*N; ++i){
        if(good[i]) goodCount++;
    }
    
    vector<long long> reverseAnswers;
    reverseAnswers.reserve(Q);
    reverseAnswers.push_back(goodCount);
    
    for(int i = Q-1; i >=0; --i){
        int r = queries[i].r;
        int c = queries[i].c;
        char d = queries[i].dir;
        int idx = indexOf(r, c);
        
        grid[idx] = '?';
        
        for(int dir=0; dir<DIRS; ++dir){
            int source_r = r + DR[dir];
            int source_c = c + DC[dir];
            int source;
            bool is_outside = false;
            if(source_r >= 0 && source_r < N && source_c >=0 && source_c < N){
                source = indexOf(source_r, source_c);
            }
            else{
                source = N*N;
                is_outside = true;
            }
            
            adj[source].push_back(idx);
            
            if(good[source] && !good[idx]){
                queue<int> bfs_q;
                bfs_q.push(idx);
                good[idx] = true;
                goodCount++;
                while(!bfs_q.empty()){
                    int u = bfs_q.front(); bfs_q.pop();
                    for(auto &v : adj[u]){
                        if(!good[v]){
                            good[v] = true;
                            goodCount++;
                            bfs_q.push(v);
                        }
                    }
                }
            }
        }
        
        reverseAnswers.push_back(goodCount);
    }
    
    vector<long long> answers(Q);
    for(int i=0; i<Q; ++i){
        answers[i] = reverseAnswers[Q -1 -i];
    }
    
    for(int i=0; i<Q; ++i){
        cout << (long long)(N*N) - answers[i] << '\n';
    }
    
    return 0;
}
