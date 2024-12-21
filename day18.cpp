#include <climits>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cassert>
#include <utility>
#include <vector>

using namespace std;

#define N 71
#define NBB 1024

vector<vector<int>> map;
vector<pair<int, int>> bytes;

bool coord_ok(int i, int j) { return i >= 0 && j >= 0 && i < N && j < N; }

pair<int, int> neighbors[]{
    {1, 0},
    {-1, 0},
    {0, 1},
    {0, -1}
};

void _path_finding(int i, int j, int dist, vector<vector<int>>& mem) {
    if (dist < mem[i][j]) {
        mem[i][j] = dist;
        for (auto [di, dj] : neighbors) {
            if (coord_ok(i+di, j+dj) && map[i+di][j+dj] != '#')
                _path_finding(i+di, j+dj, dist+1, mem);
        }
    }
}

void color_path(int i, int j, const vector<vector<int>>& mem, vector<vector<bool>>& fill) {
    fill[i][j] = true;
    if (i == 0 && j == 0) return;
    for (auto [di, dj] : neighbors) {
        if (coord_ok(i+di, j+dj) && mem[i+di][j+dj] == mem[i][j]-1) {
            color_path(i+di, j+dj, mem, fill);
            return;
        }
    }
}

int path_finding(int i, int j) {
    assert(coord_ok(i, j));
    auto mem = vector<vector<int>>(N, vector<int>(N, INT_MAX));
    _path_finding(i, j, 0, mem);
    return mem[N-1][N-1];
}

bool path_exists(vector<vector<bool>>& path, int X, int Y) {
    if (!path[0][0] || path[Y][X]) {
        auto mem = vector<vector<int>>(N, vector<int>(N, INT_MAX));
        _path_finding(0, 0, 0, mem);
        if (mem[N-1][N-1] == INT_MAX) return false;
        for (int i = N; i--;) {
            for (int j = N; j--;) path[i][j] = false;
        }
        color_path(N-1, N-1, mem, path);
    }
    return true;
}

int main(int argc, char** argv)
{
    assert(argc==2);
    ifstream file{argv[1]};
    for (int X, Y; file >> X;) {
        char c;
        file >> c; file >> Y;
        bytes.push_back({X, Y});
    }
    map = vector<vector<int>>(N, vector<int>(N, '.'));

    for (int i = 0; i < NBB; i++) {
        auto [X, Y] = bytes[i];
        map[Y][X] = '#';
    }

    cout << "part1: " << path_finding(0, 0) << endl;

    int nbb = NBB;
    auto path = vector<vector<bool>>(N, vector<bool>(N, false));
    while (true) {
        auto [X, Y] = bytes[nbb];
        map[Y][X] = '#';
        if (!path_exists(path, X, Y)) break;
        nbb++;
    }

    auto [X, Y] = bytes[nbb];

    cout << "part2: " << X << ',' << Y << endl;

    return 0;
}
