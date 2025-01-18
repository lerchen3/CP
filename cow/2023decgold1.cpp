#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n;
    cin >> n;
    
    // Read input into a 2D array
    vector<vector<int>> num_flight_routes(n, vector<int>(n));
    for (int i = 0; i < n-1; i++) {
        string line;
        cin >> line;
        for (int j = i+1; j < n; j++) {
            if (j-i-1 < line.length()) {
                num_flight_routes[i][j] = line[j-i-1] - '0';
            } else {
                num_flight_routes[i][j] = 0;  // Fill with zeros if line is shorter
            }
        }
    }
    
    int answer = 0;
    // Process each starting city l
    for (int l = 0; l < n; l++) {
        vector<int> points_to;  // Cities that l has direct flights to
        
        // Check each possible destination r
        for (int r = l+1; r < n; r++) {
            int running_mod_sum = 0;
            
            // Add up contributions from intermediate paths
            for (int k : points_to) {
                running_mod_sum = (running_mod_sum + num_flight_routes[k][r]) % 2;
            }
            
            // Add the total number of routes from l to r
            running_mod_sum = (running_mod_sum + num_flight_routes[l][r]) % 2;
            
            // If odd number of routes remains, there must be a direct flight
            if (running_mod_sum == 1) {
                points_to.push_back(r);
                answer++;
            }
        }
    }
    
    cout << answer << endl;
    return 0;
}
