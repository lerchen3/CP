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

    void update(int idx) {
        idx += size;
        tree[idx] +=1;
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
    
    freopen("bphoto.in", "r", stdin);
    freopen("bphoto.out", "w", stdout);

    int N;
    cin >> N;
    vector<int> H(N);
    for(auto &x: H) cin >> x;

    vector<int> sorted_H = H;
    sort(sorted_H.begin(), sorted_H.end(), greater<int>()); // Descending order
    unordered_map<int, int> height_to_rank;
    int rank =1;
    for(auto h: sorted_H){
        if(height_to_rank.find(h) == height_to_rank.end()){
            height_to_rank[h] = rank++;
        }
    }

    vector<long long> L(N, 0LL);
    SegmentTree ST_Left(N);
    for(int i=0; i<N; i++){
        int current_rank = height_to_rank[H[i]];
        if(current_rank >1){
            L[i] = ST_Left.query(0, current_rank-1);
        }
        ST_Left.update(current_rank-1);
    }

    vector<long long> R_count(N, 0LL);
    SegmentTree ST_Right(N);
    for(int i=N-1; i>=0; i--){
        int current_rank = height_to_rank[H[i]];
        if(current_rank >1){
            R_count[i] = ST_Right.query(0, current_rank-1);
        }
        ST_Right.update(current_rank-1);
    }

    int unbalanced =0;
    for(int i=0; i<N; i++){
        long long Li = L[i];
        long long Ri = R_count[i];
        if(Li ==0 && Ri ==0){
            continue;
        }
        if(Li ==0 || Ri ==0){
            unbalanced++;
            continue;
        }
        if(max(Li, Ri) > 2 * min(Li, Ri)){
            unbalanced++;
        }
    }

    cout << unbalanced;

    return 0;
}
