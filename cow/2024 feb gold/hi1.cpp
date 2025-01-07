#include <vector>
#include <set>
#include <iostream>
#include <tuple>

using namespace std;
typedef long long ll;

ll ceil_div(ll a, ll b) {
    return (a + b - 1) / b;
}

ll get_collision_time(ll pos_i, ll pos_j, ll speed_i, ll speed_j, bool is_even) {
    ll val = ceil_div(pos_j - pos_i, speed_i + speed_j);
    return is_even ? 2 * val - 1 : 2 * val;
}

void solve() {
    int N;
    cin >> N;
    
    vector<ll> pos(N), speed(N);
    for(int i = 0; i < N; i++) cin >> pos[i];
    for(int i = 0; i < N; i++) cin >> speed[i];
    
    set<int> alive;
    for(int i = 0; i < N; i++) alive.insert(i);
    
    vector<ll> ans(N);
    set<tuple<ll,int,int>> collisions;
    
    for(int i = 0; i < N-1; i++) {
        bool is_even = (i % 2 == 0);
        ll collision_time = get_collision_time(pos[i], pos[i+1], speed[i], speed[i+1], is_even);
        collisions.insert({collision_time, i, i+1});
    }
    
    while(!collisions.empty()) {
        vector<tuple<ll,int,int>> contenders;
        auto [cur_time, first_i, first_j] = *collisions.begin();
        collisions.erase(collisions.begin());
        contenders.push_back({cur_time, first_i, first_j});
        
        while(!collisions.empty() && get<0>(*collisions.begin()) == cur_time) {
            contenders.push_back(*collisions.begin());
            collisions.erase(collisions.begin());
        }
        
        for(auto [t, i, j] : contenders) {
            if(alive.count(i) && alive.count(j)) {
                ans[i] = ans[j] = t;
                alive.erase(i);
                alive.erase(j);
                
                auto it_i = alive.lower_bound(i);
                auto it_j = alive.upper_bound(j);
                
                if(it_i != alive.begin() && it_j != alive.end()) {
                    int new_i = *prev(it_i);
                    int new_j = *it_j;
                    bool is_even = (new_i % 2 == 0);
                    ll new_collision_time = get_collision_time(pos[new_i], pos[new_j], speed[new_i], speed[new_j], is_even);
                    collisions.insert({new_collision_time, new_i, new_j});
                }
            }
        }
    }
    
    for(int i = 0; i < N; i++) {
        cout << ans[i];
        if(i != N-1) cout << " ";
    }
    cout << "\n";
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    
    int T;
    cin >> T;
    while(T--) solve();
    return 0;
}