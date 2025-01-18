#include <iostream>
#include <vector>
#include <set>
#include <stack>
#include <map>
#include <algorithm>
using namespace std;

int main() {
    long long n, m;
    cin >> n >> m;

    vector<vector<long long>> roads(m, vector<long long>(3));
    for (long long i = 0; i < m; i++) {
        cin >> roads[i][0] >> roads[i][1] >> roads[i][2];
    }
    long long one = roads[0][2];

    vector<set<pair<long long, long long>>> adj(n+1);
    vector<set<pair<long long, long long>>> adjreversed(n+1);
    map<long long, long long> labels;  // key: v*(n+1) + w, value: label
    
    for (long long i = 0; i < m; i++) {
        adj[roads[i][0]].insert({roads[i][1], roads[i][2]});
        adjreversed[roads[i][1]].insert({roads[i][0], roads[i][2]});
        labels[roads[i][0]*(n+1) + roads[i][1]] = roads[i][2];
    }
    deque<long long> queue;
    vector<long long> indegree(n+1, 0);
    for (long long i = 1; i <= n; i++) {
        for (auto [w, label] : adjreversed[i]) {
            indegree[i] += 1;
        }
    }
    vector<long long> top_order;
    for (long long i = 1; i <= n; i++) {
        if(indegree[i] == 0) {
            queue.push_back(i);
            top_order.push_back(i);
        }
    }
    while (!queue.empty()) {
        long long v = queue.front();
        queue.pop_front();
        for (auto [w, label] : adj[v]) {
            indegree[w] -= 1;
            if(indegree[w] == 0) {
                queue.push_back(w);
                top_order.push_back(w);
            }
        }
    }

    vector<long long> max_path_length(n+1, 0);
    for (long long i = top_order.size()-1; i >= 0; i-- ) {
        for (auto [w, label] : adj[top_order[i]]) {
            max_path_length[top_order[i]] = max(max_path_length[top_order[i]], max_path_length[w] + 1);
        }
    }

    vector<vector<long long>> buckets(n);
    for (long long i = 1; i <= n; i++) {
        buckets[max_path_length[i]].push_back(i);
    }

    vector<long long> order(n+1, 0);
    vector<long long> sum_path_lengths(n+1, 0);
    auto get_label = [&](long long v, long long w) {
        return labels[v*(n+1) + w];
    };
    for (long long i = 1; i < n; i++) {
        vector<tuple<long long, long long>> processed;
        for (long long v : buckets[i]) {
            long long wmin = 0;
            for (auto [w, label] : adj[v]) {
                if(max_path_length[w] != (i-1)) {
                    continue;
                }
                if(wmin == 0) {
                    wmin = w;
                }
                if(get_label(v, wmin) > get_label(v, w)) {
                    wmin = w;
                }
                if(get_label(v, wmin) == get_label(v, w)) {
                    if(order[wmin] > order[w]) {
                        wmin = w;
                    }
                }
            }
            sum_path_lengths[v] = sum_path_lengths[wmin] + get_label(v, wmin);
            processed.push_back({v, wmin});
        }

        sort(processed.begin(), processed.end(), [&](tuple<long long, long long> a, tuple<long long, long long> b) {
            long long v1 = get<0>(a), v2 = get<0>(b);
            long long w1 = get<1>(a), w2 = get<1>(b);
            if(get_label(v1, w1) != get_label(v2, w2)) {
                return get_label(v1, w1) < get_label(v2, w2);
            }
            return order[w1] < order[w2];
        });
        long long idx = 0;
        long long prev_v = -1;
        long long prev_w = -1;
        for (long long j = 0; j < processed.size(); j++) {
            long long v = std::get<0>(processed[j]);
            long long w = std::get<1>(processed[j]);
            if(j == 0) {
                order[v] = idx;
                prev_v = v;
                prev_w = w;
            }
            else {
                if(get_label(prev_v, prev_w) != get_label(v, w) || order[prev_w] != order[w]) {
                    idx += 1;
                }
                order[v] = idx;
                prev_v = v;
                prev_w = w;
            }
        }
    }
    for (long long i = 1; i <= n; i++) {
        if(i == n){
            cout << max_path_length[i] << " " << sum_path_lengths[i];
        }
        else {
            cout << max_path_length[i] << " " << sum_path_lengths[i] << "\n";
        }
    }
    
}