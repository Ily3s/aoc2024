#include <climits>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cassert>
#include <utility>
#include <vector>

using namespace std;

vector<vector<int>> map;
vector<vector<bool>> visited;
vector<vector<int>> ratings;
int n, p;

bool coord_ok(int a, int b) {
    return a >= 0 && b >= 0 && a < n && b < p;
}

pair<int, int> neighbors[]{
    {1, 0},
    {0, 1},
    {-1, 0},
    {0, -1}
};

int parcours(int i, int j, int k) {
    if (!coord_ok(i, j) || map[i][j] != k) return 0;
    if (visited[i][j]) return 0;
    visited[i][j] = true;
    if (map[i][j] == 9) return 1;
    int res = 0;
    for (auto [r, s] : neighbors)
        res += parcours(i+r, j+s, k+1);
    return res;
}

int parcours2(int i, int j, int k) {
    if (!coord_ok(i, j) || map[i][j] != k) return 0;
    if (ratings[i][j] != -1) return ratings[i][j];
    if (map[i][j] == 9) return 1;
    int res = 0;
    for (auto [r, s] : neighbors)
        res += parcours2(i+r, j+s, k+1);
    ratings[i][j] = res;
    return res;
}

int main(int argc, char** argv)
{
    assert(argc==2);
    ifstream file{argv[1]};
    string line;
    while (getline(file, line)) {
        map.push_back({});
        for (int i = 0; i < line.size(); i++)
            map.back().push_back(line[i]-'0');
    }
    n = map.size(); p = map[0].size();

    ratings = vector<vector<int>>(n, vector<int>(p, -1));

    int output1 = 0, output2 = 0;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < p; j++) {
            if (map[i][j] == 0) {
                visited = vector<vector<bool>>(n, vector<bool>(p, false));
                output1 += parcours(i, j, 0);
                output2 += parcours2(i, j, 0);
            }
        }
    }

    std::cout << "part1: " << output1 << std::endl;
    std::cout << "part2: " << output2 << std::endl;

    return 0;
}
