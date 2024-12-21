#include <climits>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <vector>

using namespace std;

vector<string> map;
vector<vector<int>> dists;
int n, p;
int iS=0, jS=0, iE=0, jE=0;

pair<int, int> neighbors[]{
    {1, 0},
    {0, 1},
    {-1, 0},
    {0, -1}
};

// for part 1, the points with a distance of 2 from origin
pair<int, int> sec_neighbs[]{
    {2, 0},
    {0, 2},
    {-2, 0},
    {0, -2},
    {1, 1},
    {-1, -1},
    {1, -1},
    {-1, 1}
};

bool coord_ok(int i, int j) { return i >= 0 && j >= 0 && i < n && j < p; }

void path_finding(int i, int j, int dist) {
    if (dist < dists[i][j]) {
        dists[i][j] = dist;
        for (auto [di, dj] : neighbors) {
            if (coord_ok(i+di, j+dj) && map[i+di][j+dj] != '#')
                path_finding(i+di, j+dj, dist+1);
        }
    }
}

int dist1(int i1, int j1, int i2, int j2) {
    return abs(i1-i2)+abs(j1-j2);
}

template<typename T>
uint64_t solve(const T& region) {
    uint64_t res = 0;

    for (int i = n; i--;) {
        for (int j = p; j--;) {
            if (dists[i][j] == INT_MAX) continue;
            for (auto [di, dj] : region) {
                if (coord_ok(i+di, j+dj) && dists[i+di][j+dj] != INT_MAX)
                    res += dists[i+di][j+dj]-dists[i][j]-dist1(i, j, i+di, j+dj) >= 100;
            }
        }
    }
    return res;
}

int main(int argc, char** argv)
{
    // input parsing
    assert(argc==2);
    ifstream file{argv[1]};
    string line;
    while (getline(file, line)) {
        if (line.size() <= 1) break;
        if (line.back() == '\r') line.pop_back();
        for (int j = line.size(); j--;) {
            jS += j*(line[j]=='S');
            jE += j*(line[j]=='E');
            iS += map.size()*(line[j]=='S');
            iE += map.size()*(line[j]=='E');
        }
        map.push_back(line);
    }
    n = map.size(); p = map[0].size();

    // store the distance from S in dists
    dists = vector<vector<int>>(n, vector<int>(p, INT_MAX));
    path_finding(iS, jS, 0);

    cout << "part1: " << solve(sec_neighbs) << endl;

    // all points with a distance of less than 20 from origin
    vector<pair<int, int>> radius20;
    for (int di = -20; di <= 20; di++) {
        for (int dj = -20; dj <= 20; dj++) {
            if (dist1(0, 0, di, dj) <= 20) radius20.push_back({di, dj});
        }
    }

    cout << "part2: " << solve(radius20) << endl;
    
    return 0;
}
