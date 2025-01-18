#include <iostream>
#include <vector>
#include <algorithm>
#include <climits> // Added for LLONG_MAX
using namespace std;
typedef long long ll;

ll sumr(ll c, ll d) {
    if(c > d) return 0;
    return (d - c + 1) * (c + d) / 2;
}

void solve() {
    int n;
    cin >> n;
    vector<ll> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    sort(a.begin(), a.end());
    
    ll ans = LLONG_MAX; // Fixed by including <climits>
    
    ans = min(ans, (ll)n * (n + 1) / 2);
    ans = min(ans, a[n-1]);
    ll min_p = n;
    for (int i = 0; i < n-1; i++) {
        // binary search for first p such that sumr(n-1-i, p) > a[i]
        ll l = n-1-i, r = n;
        while(l < r) {
            ll mid = (l + r) / 2;
            if(sumr(n-1-i, mid) > a[i]) r = mid;
            else l = mid + 1;
        }
        min_p = min(min_p, l);
    }
    ans = min(ans, (ll)min_p * (min_p + 1) / 2);

    cout << ans << "\n";
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}