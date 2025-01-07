#include <bits/stdc++.h>
using namespace std;

struct SegmentTree {
    int size;
    vector<long long> tree;

    SegmentTree(int n) {
        size = 1;
        while (size < n) size <<=1;
        tree.assign(2*size, 0LL);
    }

    void update(int idx, long long val) {
        idx += size;
        tree[idx] += val;
        while(idx >1){
            idx /=2;
            tree[idx] = tree[2*idx] + tree[2*idx+1];
        }
    }

    long long query(int l, int r) const {
        long long res =0;
        int left = l + size;
        int right = r + size;
        while(left < right){
            if(left&1){
                res += tree[left++];
            }
            if(right&1){
                right--;
                res += tree[right];
            }
            left /=2;
            right /=2;
        }
        return res;
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    freopen("haircut.in", "r", stdin);
    freopen("haircut.out", "w", stdout);

    int N;
    cin >> N;
    vector<int> A(N);
    for(int &x: A) cin >> x;

    vector<vector<int>> value_to_indices(N+1, vector<int>());
    for(int i=0; i<N; i++) {
        value_to_indices[A[i]].push_back(i);
    }

    SegmentTree seg(N);

    vector<long long> ANS1(N+1, 0LL);

    long long running_sum =0;

    for(int x = N; x >=0; x--){
        int c = value_to_indices[x].size();
        if(c >0){
            for(auto idx: value_to_indices[x]){
                seg.update(idx, 1LL);
            }

            long long sum_queries =0;
            for(auto idx: value_to_indices[x]){
                if(idx ==0){
                    continue;
                }
                sum_queries += seg.query(0, idx);
            }

            long long adjustment = ((long long)c * (c-1)) /2;
            running_sum += (sum_queries - adjustment);
        }
        ANS1[x] = running_sum;
    }

    for(int j=0; j<N; j++){
        cout << (ANS1[0] - ANS1[j]) << "\n";
    }

    return 0;
}
