#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <utility>
#include <functional>
using namespace std;
using namespace std;

using ll = long long;

const ll MOD = 1e9+7;

ll modExp(ll b, ll e, ll m) {
    ll r = 1 % m;
    b %= m;
    while(e) {
        if(e & 1)
            r = (r * b) % m;
        b = (b * b) % m;
        e >>= 1;
    }
    return r;
}

struct Itv {
    ll l, r;
};

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    ll n; int m, q;
    cin >> n >> m >> q;
    
    vector<pair<ll,int>> ev;
    for (int i = 0; i < m; i++){
        ll l, r;
        cin >> l >> r;
        ev.push_back({l, 1});
        if(r+1 <= n)
            ev.push_back({r+1, -1});
    }
    sort(ev.begin(), ev.end());

    vector<Itv> its;
    ll cur = 0, pre = 1;
    for(auto &e : ev){
        ll pos = e.first;
        if (pre < pos){
            if(cur % 2)
                its.push_back({pre, pos-1});
        }
        cur += e.second;
        pre = pos;
        if(pre > n) break;
    }
    if (pre <= n){
        if(cur % 2)
            its.push_back({pre, n});
    }
    vector<ll> ie, op;
    for (int i = 0; i < (int)its.size(); i++){
        ie.push_back(its[i].r);
        ll len = its[i].r - its[i].l + 1;
        if(i == 0) op.push_back(len);
        else op.push_back(op.back() + len);
    }

    auto op_pre = [&](ll pos) -> ll {
        if(pos < 1) return 0;
        ll cnt = 0;
        if(its.empty()) return 0;
        int idx = upper_bound(ie.begin(), ie.end(), pos) - ie.begin();
        if(idx > 0)
            cnt += op[idx-1];
        if(idx < (int)its.size()){
            if(its[idx].l <= pos && pos <= its[idx].r)
                cnt += (pos - its[idx].l + 1);
        }
        return cnt;
    };

    auto op_rng = [&](ll l, ll r) -> ll {
        if(l > r) return 0;
        return op_pre(r) - op_pre(l-1);
    };

    ll iv2 = modExp(2, MOD-2, MOD);
    vector<ll> fi, fp, ie_f, is;
    for (int i = 0; i < (int)its.size(); i++){
        ll l = its[i].l, r = its[i].r;
        ll piv2_l = modExp(iv2, l, MOD);
        ll term = (2 * piv2_l) % MOD;
        ll len = r - l + 1;
        ll sub = modExp(iv2, len, MOD);
        ll full = (term * ((1 - sub + MOD) % MOD)) % MOD;
        fi.push_back(full);
        if(i==0) fp.push_back(full);
        else fp.push_back((fp.back() + full) % MOD);
        ie_f.push_back(r);
        is.push_back(l);
    }

    auto f_fn = [&](ll pos) -> ll {
        if(pos < 1) return 0;
        if(its.empty()) return 0;
        ll res = 0;
        int idx = upper_bound(ie_f.begin(), ie_f.end(), pos) - ie_f.begin();
        if(idx > 0)
            res = fp[idx-1];
        if(idx < (int)its.size()){
            if(its[idx].l <= pos && pos <= its[idx].r){
                ll l = its[idx].l;
                ll len = pos - l + 1;
                ll term = (2 * modExp(iv2, l, MOD)) % MOD;
                ll part = (term * ((1 - modExp(iv2, len, MOD) + MOD) % MOD)) % MOD;
                res = (res + part) % MOD;
            }
        }
        return res % MOD;
    };

    auto getVal = [&](ll a, ll b) -> ll {
        if(a > b) return 0;
        ll p2_b = modExp(2, b, MOD);
        ll dif = (f_fn(b) - f_fn(a-1)) % MOD;
        if(dif < 0) dif += MOD;
        return (p2_b * dif) % MOD;
    };

    while(q--){
        ll l, r, k;
        cin >> l >> r >> k;

        ll m = l - 1;
        if (op_rng(l, l) + (r - l) >= k) {
            ll lo = l, hi = r;
            while(lo <= hi) {
                ll mid = (lo + hi) / 2;
                ll f = op_rng(l, mid) + (r - mid);
                if(f >= k) {
                    m = mid;
                    lo = mid + 1;
                } else {
                    hi = mid - 1;
                }
            }
        }
        ll sl = r - m;
        ll x = k - (r - m);
        if(x < 0) x = 0;

        ll pp = 0;
        if(x > 0) {
            pp = (modExp(2, x, MOD) - 1 + MOD) % MOD;
            pp = (pp * modExp(2, sl, MOD)) % MOD;
        }
        ll sp = getVal(m + 1, r);

        ll ans = (pp + sp) % MOD;
        cout << ans << "\n";
    }

    return 0;
}