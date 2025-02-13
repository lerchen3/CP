#include <vector>
#include <algorithm>
#include <iostream>
#include <climits>
#include <unordered_map>
using namespace std;

static const int NOCHILD = -1;

struct NodeInfo {
    int leftChild, rightChild;
    int parent;
    long long DP[3];
    int catNat;
    long long a;
    long long costChange;
};

vector<NodeInfo> nodeInfo;
int N;
 
void recalcDP(int i) {
    NodeInfo &nd = nodeInfo[i];
    long long oldDP[3];
    for(int c=0;c<3;c++){
        oldDP[c] = nd.DP[c];
    }

    int lc = nd.leftChild, rc = nd.rightChild;
    auto costToSet = [&](int x){
        return (x == nd.catNat ? 0LL : nd.costChange);
    };

    if(lc == NOCHILD || rc == NOCHILD){
        for(int c=0;c<3;c++){
            nd.DP[c] = costToSet(c);
        }
    } else {
        long long dpL[3], dpR[3];
        for(int cL=0;cL<3;cL++) dpL[cL] = nodeInfo[lc].DP[cL];
        for(int cR=0;cR<3;cR++) dpR[cR] = nodeInfo[rc].DP[cR];

        for(int c=0;c<3;c++){
            nd.DP[c] = LLONG_MAX;
        }

        auto medianCat = [&](int x, int y, int z){
            int mi = min({x,y,z});
            int ma = max({x,y,z});
            return x + y + z - mi - ma;
        };

        for(int x=0;x<3;x++){
            long long costSet = costToSet(x);
            for(int cL=0;cL<3;cL++){
                long long costChildL = dpL[cL];
                if(costChildL == LLONG_MAX) continue;
                for(int cR=0;cR<3;cR++){
                    long long costChildR = dpR[cR];
                    if(costChildR == LLONG_MAX) continue;
                    int med = medianCat(x,cL,cR);
                    long long total = costSet + costChildL + costChildR;
                    if(total < nd.DP[med]){
                        nd.DP[med] = total;
                    }
                }
            }
        }
    }

    bool changed = false;
    for(int c=0;c<3;c++){
        if(nd.DP[c] != oldDP[c]){
            changed = true;
            break;
        }
    }
    if(changed && i!=1){
        int p = nd.parent;
        if(p != -1){
            recalcDP(p);
        }
    }
}

void updateCategory(int i, int newCat){
    if(nodeInfo[i].catNat == newCat) return;
    nodeInfo[i].catNat = newCat;
    recalcDP(i);
}

void buildDP(int i){
    NodeInfo &nd = nodeInfo[i];
    int lc = nd.leftChild, rc = nd.rightChild;
    if(lc != NOCHILD) buildDP(lc);
    if(rc != NOCHILD) buildDP(rc);
    recalcDP(i);
}

int main(){
    ios::sync_with_stdio(false); 
    cin.tie(nullptr);

    cin >> N;
    nodeInfo.resize(N+1);
    for(int i=1;i<=N;i++){
        long long aa, cc; 
        cin >> aa >> cc;
        nodeInfo[i].a = aa;
        nodeInfo[i].costChange = cc;
        nodeInfo[i].leftChild = NOCHILD;
        nodeInfo[i].rightChild = NOCHILD;
        nodeInfo[i].parent = -1;
        nodeInfo[i].catNat = 0;
        for(int c=0;c<3;c++){
            nodeInfo[i].DP[c] = 0;
        }
    }

    for(int i=2;i<=N;i++){
        int p = i/2;
        nodeInfo[i].parent = p;
        if(nodeInfo[p].leftChild == NOCHILD) nodeInfo[p].leftChild = i;
        else nodeInfo[p].rightChild = i;
    }

    vector<long long> vals; 
    vals.reserve(N);
    for(int i=1;i<=N;i++){
        vals.push_back(nodeInfo[i].a);
    }
    sort(vals.begin(), vals.end());
    vals.erase(unique(vals.begin(), vals.end()), vals.end());

    int Q; cin >> Q;
    vector<long long> mQ(Q);
    for(int i=0;i<Q;i++){
        cin >> mQ[i];
    }
    vector<pair<long long,int>> queries(Q);
    for(int i=0;i<Q;i++){
        queries[i] = { mQ[i], i };
    }
    sort(queries.begin(), queries.end(), 
         [&](auto &a, auto &b){
            return a.first > b.first;
         });

    sort(vals.begin(), vals.end(), greater<long long>());

    buildDP(1);

    vector<long long> answers(Q);

    int qPtr = 0;
    int vPtr = 0;
    long long currM = LLONG_MAX;
    
    auto processQueriesUntil = [&](long long boundaryVal){
        while(qPtr < Q && queries[qPtr].first >= boundaryVal){
            long long mm = queries[qPtr].first;
            int idxQ = queries[qPtr].second;
            answers[idxQ] = nodeInfo[1].DP[1];
            qPtr++;
        }
    };

    while(qPtr < Q){
        if(vPtr >= (int)vals.size() 
           || queries[qPtr].first > vals[vPtr]){
            long long boundaryVal = (vPtr < (int)vals.size()) 
                                      ? (vals[vPtr]+1)
                                      : -1;
            if(vPtr >= (int)vals.size()){
                boundaryVal = LLONG_MIN; 
            } else {
                boundaryVal = vals[vPtr] + 1; 
            }
            processQueriesUntil(boundaryVal);
            if(qPtr >= Q) break;
        }
        if(vPtr >= (int)vals.size()){
            while(qPtr < Q){
                long long mm = queries[qPtr].first;
                int idxQ = queries[qPtr].second;
                answers[idxQ] = nodeInfo[1].DP[1];
                qPtr++;
            }
            break;
        }

        long long X = vals[vPtr];

        static bool builtMap = false;
        static unordered_map<long long, vector<int>> valToNodes; 
        if(!builtMap){
            valToNodes.reserve(vals.size());
            valToNodes.max_load_factor(0.7f);
            for(int i=1;i<=N;i++){
                valToNodes[nodeInfo[i].a].push_back(i);
            }
            builtMap = true;
        }
        auto &vecNodes = valToNodes[X];
        for(int ndID : vecNodes){
            updateCategory(ndID, 1);
        }

        processQueriesUntil(X);

        for(int ndID : vecNodes){
            updateCategory(ndID, 2);
        }

        vPtr++;
    }

    for(long long ans : answers){
        cout << ans << "\n";
    }

    return 0;
}
