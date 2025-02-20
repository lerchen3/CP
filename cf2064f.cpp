#include <vector>
#include <set>
#include <algorithm>
#include <iostream>
#include <map>

using namespace std;

typedef long long ll;

ll helper(int minIndex, int maxIndex, const vector<int>& A, const vector<int>& B, const set<int>& C) {
    ll ans = 0;
    int prev = minIndex;
    map<int, int> dict;
    int old = maxIndex;
    int curr = 0;
    for (int i = B.size() - 1; i >= 0; i--){
        int b = B[i];
        auto it2 = C.lower_bound(b);
        if(*it2 < old){
            curr += *it2 - b;
        }
        else{
            curr += old - b;
        }
        dict[b] = curr;
        old = b;
    }
    
    for (int a : A) {
        if (a >= maxIndex)
            break;  
        auto it = std::lower_bound(B.begin(), B.end(), a + 1);
        if (it == B.end())
            break; 
        int b_min = *it;
        if (b_min >= maxIndex)
            continue;
        ans += (ll)(a - prev) * dict[b_min];
        prev = a;
    }
    return ans;
}

int main(){
    try {
        ios::sync_with_stdio(false);
        cin.tie(nullptr);
    
        int t;
        cin >> t;
        while(t--){
            try {
                int n, k;
                cin >> n >> k;
                
                vector<int> a(n + 1);
                for (int i = 1; i <= n; i++){
                    cin >> a[i];
                }
                
                vector<vector<int>> pos(n + 1);
                for (int i = 1; i <= n; i++){
                    if (a[i] < 1 || a[i] > n) continue;
                    pos[a[i]].push_back(i);
                }
                
                set<int> S1, S2;
                ll answer = 0;
                
                for (int l = 1; l <= n; l++){
                    int kl = k - l;
                    
                    vector<int>& posL = pos[l];
                    if(posL.empty() || kl < 1 || kl > n || pos[kl].empty()){
                        for (int idx : posL) {
                            S1.insert(idx);
                        }
                        if (kl >= 1 && kl <= n) {
                            for (int idx : pos[kl]) {
                                S2.insert(idx);
                            }
                        }
                        continue;
                    }
                    
                    vector<bool> visited(posL.size(), false);
                    for (int i = 0; i < (int) posL.size(); i++){
                        if(visited[i])
                            continue;
                        int idx = posL[i];
                        visited[i] = true;

                        int minIndex = 0;
                        {
                            auto it = S1.lower_bound(idx);
                            if(it != S1.begin()){
                                it--;
                                minIndex = *it;
                            }
                        }
                        int minIdx2 = n + 1;
                        {
                            auto it = S1.upper_bound(idx);
                            if(it != S1.end()){
                                minIdx2 = *it;
                            }
                        }
                        int maxIndex = n + 1;
                        {
                            auto it = S2.lower_bound(minIdx2);
                            if(it != S2.end()){
                                maxIndex = *it;
                            }
                        }

                        vector<int> blockA;
                        blockA.push_back(idx);
                        int j = i + 1;
                        while(j < (int) posL.size() && posL[j] < minIdx2) {
                            visited[j] = true;
                            blockA.push_back(posL[j]);
                            j++;
                        }

                        vector<int> blockB;
                        vector<int>& posKL = pos[kl];
                        auto lb = std::lower_bound(posKL.begin(), posKL.end(), idx);
                        while(lb != posKL.end() && *lb < maxIndex) {
                            blockB.push_back(*lb);
                            lb++;
                        }

                        set<int> blockC;
                        for (int idx : blockB){
                            auto it = S2.lower_bound(idx);
                            if(it != S2.end()){
                                blockC.insert(*it);
                            }
                        }
                        blockC.insert(maxIndex);

                        ll cnt = helper(minIndex, maxIndex, blockA, blockB, blockC);
                        answer += cnt;
                    }

                    for (int idx : posL) {
                        S1.insert(idx);
                    }
                    for (int idx : pos[kl]) {
                        S2.insert(idx);
                    }
                }
                
                cout << answer << "\n";
                cout.flush();
            } catch (const exception& e) {
            } catch (...) {
            }
        }
    } catch (const exception& e) {
        return 1;
    } catch (...) {
        return 1;
    }
    
    return 0;
}