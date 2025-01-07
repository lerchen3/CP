#include <bits/stdc++.h>
using namespace std;

static const long long NEG_INF = -10000000010LL; 
static const long long POS_INF =  10000000010LL;

vector<long long> getDistinctSorted(const vector<long long> &arr) {
    vector<long long> tmp = arr;
    sort(tmp.begin(), tmp.end());
    tmp.erase(unique(tmp.begin(), tmp.end()), tmp.end());
    return tmp;
}

int bucketIndex(long long x, const vector<long long> &c) {
    int M = (int)c.size();
    if (x < c[0]) return 0;
    if (x > c[M-1]) return 2*M;
    int i = int(std::lower_bound(c.begin(), c.end(), x) - c.begin());
    if (c[i] == x) return 2*i + 1;
    return 2*i;
}

int getLeftBucket(long long l, const vector<long long> &c) {
    return bucketIndex(l, c);
}

int getRightBucket(long long r, const vector<long long> &c) {
    return bucketIndex(r, c);
}

struct Interval {
    long long l, r, t;
    int Lb, Rb;
    long long capacity;
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    while(T--) {
        int N, K;
        cin >> N >> K;

        vector<long long> treePos(N);
        for(int i=0; i<N; i++){
            cin >> treePos[i];
        }

        vector<Interval> intervals(K);
        vector<long long> allEnds;
        allEnds.reserve(2*K);

        for(int i=0; i<K; i++){
            long long l, r, t;
            cin >> l >> r >> t;
            intervals[i].l = l;
            intervals[i].r = r;
            intervals[i].t = t;
            intervals[i].Lb = 0;
            intervals[i].Rb = 0;
            intervals[i].capacity = 0;
            allEnds.push_back(l);
            allEnds.push_back(r);
        }

        vector<long long> c = getDistinctSorted(allEnds);
        int M = (int)c.size();
        int totalBuckets = 2*M + 1;

        vector<long long> BUCKETS(totalBuckets, 0LL);

        for(auto &xval : treePos){
            int b = bucketIndex(xval, c);
            BUCKETS[b]++;
        }

        vector<long long> pref(totalBuckets+1, 0LL);
        for(int i=0; i<totalBuckets; i++){
            pref[i+1] = pref[i] + BUCKETS[i];
        }

        for(int i=0; i<K; i++){
            intervals[i].Lb = getLeftBucket(intervals[i].l, c);
            intervals[i].Rb = getRightBucket(intervals[i].r, c);
            long long sumRange = pref[intervals[i].Rb + 1] - pref[intervals[i].Lb];
            intervals[i].capacity = sumRange - intervals[i].t;
        }

        vector<vector<int>> startCover(totalBuckets), endCover(totalBuckets);
        for(int i=0; i<K; i++){
            int Lb = intervals[i].Lb;
            int Rb = intervals[i].Rb;
            if(Lb <= Rb) {
                startCover[Lb].push_back(i);
                if(Rb+1 < totalBuckets){
                    endCover[Rb+1].push_back(i); 
                }
            }
        }

        vector<long long> capLeft(K);
        for(int i=0; i<K; i++){
            capLeft[i] = intervals[i].capacity;
        }

        multiset<pair<long long,int>> active;
        unordered_map<int, multiset<pair<long long, int>>::iterator> id_to_it;
        long long answer = 0;
        long long running_deletion = 0;

        for(int b=0; b<totalBuckets; b++){
            for(int id : endCover[b]){
                auto it = id_to_it.find(id);
                if(it != id_to_it.end()){
                    active.erase(it->second);
                    id_to_it.erase(it);
                }
            }

            for(int id : startCover[b]){
                auto it = active.emplace(capLeft[id] + running_deletion, id);
                id_to_it[id] = it;
            }

            if(!active.empty()){
                auto it = active.begin();
                long long adjusted_cap = it->first;
                int interval_id = it->second;

                long long D = min(BUCKETS[b], adjusted_cap - running_deletion);
                answer += D;

                running_deletion += D;
            }
            else{
                answer += BUCKETS[b];
            }
        }

        cout << answer << "\n";
    }

    return 0;
}