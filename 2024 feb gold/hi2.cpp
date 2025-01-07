#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <utility>

using namespace std;
typedef long long ll;

ll distance(ll x, ll y, ll N) {
    ll result = (y >= x) ? y - x : N - (x - y);
    return result;
}

ll find_bigger(ll x, const set<ll>& T, ll N) {
    auto it = T.upper_bound(x);
    ll result = (it == T.end()) ? *T.begin() : *it;
    return result;
}

ll find_smaller(ll x, const set<ll>& T, ll N) {
    if (T.empty()) {
        return 0;
    }
    auto it = T.lower_bound(x);
    ll result = (it == T.begin()) ? *T.rbegin() : *prev(it);
    return result;
}

int main() {
    ll N;
    cin >> N;
    
    vector<ll> a(N);
    for (int i = 0; i < N; i++) {
        cin >> a[i];
    }
    
    vector<pair<ll, ll>> S;
    for (ll i = 0; i < N; i++) {
        S.push_back({a[i], i});
    }
    
    sort(S.begin(), S.end());
    
    vector<ll> order(N);
    for (ll i = 0; i < N; i++) {
        order[i] = S[i].second;
    }
    
    vector<ll> velocity_change_BEFORE(N + 1, 0);
    ll initial_velocity = 0;
    ll initial_milk = 0;
    for (ll x : a) {
        initial_velocity += x;
        initial_milk += x;
    }
    
    set<ll> T;
    for (ll order_idx = 0; order_idx < N; order_idx++) {
        ll i = order[order_idx];
        
        if (order_idx == 0) {
            T.insert(i);
            velocity_change_BEFORE[N] -= a[i];
            continue;
        }
        
        ll L = find_smaller(i, T, N);
        ll R = find_bigger(i, T, N);
        
        ll dist_L_i = distance(L, i, N);
        ll dist_i_R = distance(i, R, N);
        
        velocity_change_BEFORE[dist_L_i] -= a[i];
        velocity_change_BEFORE[dist_i_R] -= a[i];
        velocity_change_BEFORE[dist_i_R + dist_L_i] += a[i];
        T.insert(i);
    }
    
    ll cur_ans = initial_milk;
    ll cur_velocity = initial_velocity;
    
    for (ll j = 1; j <= N; j++) {
        cur_velocity += velocity_change_BEFORE[j];
        cur_ans += cur_velocity;
        cout << cur_ans << "\n";
    }
    
    return 0;
} 