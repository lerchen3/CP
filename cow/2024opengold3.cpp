#include <vector>
#include <iostream>
#include <algorithm>
#include <cstring>

using namespace std; 
static const long long MOD = 1000000007; 
long long pA[501], pB[501]; 
long long dp[501][501]; 

int main(){ 
    ios::sync_with_stdio(false); 
    cin.tie(nullptr); 

    int N; 
    cin >> N; 

    vector<long long> a(N), b(N); 
    for(int i=0;i<N;i++) {
        cin >> a[i];
    }
    for(int i=0;i<N;i++) {
        cin >> b[i];
    }

    for(int i=1;i<=N;i++){ 
        pA[i]=pA[i-1]+a[i-1]; 
        pB[i]=pB[i-1]+b[i-1]; 
    }

    auto getA = [&](int l,int r)-> long double { 
        return (long double)(pA[r+1]-pA[l])/(r-l+1);
    }; 
    auto getB = [&](int l,int r)-> long double { 
        return (long double)(pB[r+1]-pB[l])/(r-l+1);
    }; 

    memset(dp,0,sizeof(dp)); 

    for(int i=0;i<N;i++){ 
        if(getA(i,N-1)<=getB(N-1,N-1)) {
            dp[i][N-1]=1; 
        }
    } 

    for(int j=0;j<N;j++){ 
        if(getA(N-1,N-1)<=getB(j,N-1)) {
            dp[N-1][j]=1; 
        }
    } 

    for(int j=N-2;j>=0;j--){ 
        vector<pair<long double,int>> v; 
        for(int D=j;D<N-1;D++){ 
            v.push_back({getB(j,D),D}); 
        } 
        sort(v.begin(),v.end(),[&](auto &x,auto &y)->bool { 
            return x.first < y.first;
        }); 

        vector<int> order(v.size()); 
        for(int x=0;x<(int)v.size();x++) {
            order[x]=v[x].second; 
        }

        vector<vector<long long>> sum_dp(N, vector<long long>(v.size()));
        
        for(int I=N-2; I>=0; I--){
            for(int x=v.size()-1; x>=0; x--){
                if(x == v.size()-1) {
                    sum_dp[I][x] = dp[I+1][order[x] + 1];
                } else {
                    sum_dp[I][x] = (sum_dp[I][x+1] + dp[I+1][order[x] + 1]) % MOD;
                }
            }

            for(int C=I; C<N-1; C++){
                long double val = getA(I,C);
                int left=0, right=(int)v.size()-1, pos=v.size();
                while(left<=right){
                    int mid=(left+right)/2;
                    if(v[mid].first >= val){
                        pos=mid;
                        right=mid-1;
                    }
                    else{
                        left=mid+1;
                    }
                }
                if(pos == (int)v.size()){
                }
                else{
                    dp[I][j] = (dp[I][j] + sum_dp[C][pos]) % MOD;
                }
            }

            if(getA(I,N-1)<=getB(j,N-1)){
                dp[I][j] = (dp[I][j] + 1) % MOD;
            }
        }
    } 
    cout << dp[0][0]%MOD << "\n"; 
    return 0; 
}