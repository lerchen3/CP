#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

const int MOD = 998244353;

int mergeVal(int a, int b) {
    if(a == -2 || b == -2) return -2;
    if(a == -1) return b;
    if(b == -1) return a;
    return (a == b ? a : -2);
}

struct CustomSegTree {
    int n;
    vector<int> seg;
    
    CustomSegTree(int n): n(n) {
        seg.assign(4 * n, -1);
        build(1, 1, n);
    }
    
    void build(int idx, int l, int r) {
        if(l == r) {
            seg[idx] = -1;
            return;
        }
        int mid = (l + r) / 2;
        build(2 * idx, l, mid);
        build(2 * idx + 1, mid + 1, r);
        seg[idx] = mergeVal(seg[2 * idx], seg[2 * idx + 1]);
    }
    
    void update(int idx, int l, int r, int pos, int val) {
        if(l == r) {
            seg[idx] = val;
            return;
        }
        int mid = (l + r) / 2;
        if(pos <= mid)
            update(2 * idx, l, mid, pos, val);
        else
            update(2 * idx + 1, mid + 1, r, pos, val);
        seg[idx] = mergeVal(seg[2 * idx], seg[2 * idx + 1]);
    }
    
    void update(int pos, int val) {
        update(1, 1, n, pos, val);
    }
    
    int rangeQueryUtil(int idx, int l, int r, int ql, int qr) {
        if(ql > r || qr < l)
            return -1;
        if(ql <= l && r <= qr)
            return seg[idx];
        int mid = (l + r) / 2;
        int leftVal = rangeQueryUtil(2 * idx, l, mid, ql, qr);
        int rightVal = rangeQueryUtil(2 * idx + 1, mid + 1, r, ql, qr);
        return mergeVal(leftVal, rightVal);
    }
    
    int rangeQuery(int ql, int qr) {
        return rangeQueryUtil(1, 1, n, ql, qr);
    }
    
    int findRightmostCandidate(int ql, int qr, int desired) {
        int candidate = -1;
        int lo = ql, hi = qr;
        while(lo <= hi) {
            int mid = (lo + hi) / 2;
            int merged = rangeQuery(mid, qr);
            if(merged == desired || merged == -1)
                hi = mid - 1;
            else {
                candidate = mid;
                lo = mid + 1;
            }
        }
        return candidate;
    }
    
    int findLeftmostCandidate(int ql, int qr, int desired) {
        int candidate = -1;
        int lo = ql, hi = qr;
        while(lo <= hi) {
            int mid = (lo + hi) / 2;
            int merged = rangeQuery(ql, mid);
            if(merged == desired || merged == -1)
                lo = mid + 1;
            else {
                candidate = mid;
                hi = mid - 1;
            }
        }
        return candidate;
    }
};

struct Fenw {
    int n;
    vector<int> f;
    Fenw(int n): n(n) {
        f.assign(n, 0);
    }
    
    void update(int i, int delta) {
        for(; i < n; i = i | (i + 1))
            f[i] += delta;
    }
    
    int query(int i) {
        int s = 0;
        for(; i >= 0; i = (i & (i + 1)) - 1)
            s += f[i];
        return s;
    }
    
    int queryRange(int l, int r) {
        if(l > r) return 0;
        return query(r) - (l ? query(l - 1) : 0);
    }
    
    int lower_bound(int target) {
        int idx = 0;
        int bit = 1;
        while(bit < n) bit <<= 1;
        for(; bit; bit >>= 1) {
            int next = idx + bit;
            if(next - 1 < n && f[next - 1] < target) {
                target -= f[next - 1];
                idx = next;
            }
        }
        return idx;
    }
    
    int find_first_in_range(int l, int r) {
        int sum_before = (l == 0 ? 0 : query(l - 1));
        int target = sum_before + 1;
        int pos = lower_bound(target);
        if(pos <= r) return pos;
        return -1;
    }
};

struct Interval {
    int l, r;
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while(t--){
        int n;
        cin >> n;
        vector<int> p(n + 1), c(n + 1);
        for (int i = 1; i <= n; i++)
            cin >> p[i];
        for (int i = 1; i <= n; i++)
            cin >> c[i];

        vector<int> L_bound(n + 1), R_bound(n + 1);

        vector<int> order(n);
        for (int i = 0; i < n; i++)
            order[i] = i + 1;
        sort(order.begin(), order.end(), [&](int a, int b) {
            return p[a] > p[b];
        });

        CustomSegTree segLower(n);
        for (int idx : order) {
            if(idx > 1) {
                int candidate = segLower.findRightmostCandidate(1, idx - 1, c[idx]);
                if(candidate == -1)
                    L_bound[idx] = 1;
                else
                    L_bound[idx] = candidate + 1;
            } else {
                L_bound[idx] = 1;
            }
            segLower.update(idx, c[idx]);
        }

        CustomSegTree segUpper(n);
        for (int idx : order) {
            if(idx < n) {
                int candidate = segUpper.findLeftmostCandidate(idx + 1, n, c[idx]);
                if(candidate == -1)
                    R_bound[idx] = n;
                else
                    R_bound[idx] = candidate - 1;
            } else {
                R_bound[idx] = n;
            }
            segUpper.update(idx, c[idx]);
        }

        vector<vector<int>> groups(n + 1);
        for (int i = 1; i <= n; i++) {
            groups[c[i]].push_back(i);
        }
        for (int color = 1; color <= n; color++) {
            if(!groups[color].empty())
                sort(groups[color].begin(), groups[color].end());
        }

        long long ans = 1;

        for (int color = 1; color <= n; color++){
            if(groups[color].empty()) continue;
            int m = groups[color].size();
            vector<Interval> intervals;
            for (int pos : groups[color]) {
                int l_c = int(lower_bound(groups[color].begin(), groups[color].end(), L_bound[pos]) - groups[color].begin());
                int r_c = int(upper_bound(groups[color].begin(), groups[color].end(), R_bound[pos]) - groups[color].begin()) - 1;
                if(l_c > r_c) {
                    ans = 0;
                    break;
                }
                intervals.push_back({l_c, r_c});
            }
            if(ans == 0)
                break;

            sort(intervals.begin(), intervals.end(), [](const Interval &a, const Interval &b) {
                int la = a.r - a.l + 1, lb = b.r - b.l + 1;
                if(la != lb) return la < lb;
                return a.l < b.l;
            });

            Fenw fenw(m);
            for (int i = 0; i < m; i++)
                fenw.update(i, 1);

            for (auto &inter : intervals) {
                int free_count = fenw.queryRange(inter.l, inter.r);
                if(free_count <= 0) {
                    ans = 0;
                    break;
                }
                ans = (ans * free_count) % MOD;
                int pos = fenw.find_first_in_range(inter.l, inter.r);
                if(pos == -1) { ans = 0; break; }
                fenw.update(pos, -1);
            }
            if(ans == 0)
                break;
        }

        cout << ans % MOD << "\n";
    }
    return 0;
}