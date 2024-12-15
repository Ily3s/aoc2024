#include <array>
#include <climits>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cassert>
#include <sstream>
#include <vector>
#include <cstdint>

using namespace std;

vector<string> map;
vector<char> instructions;
int n, p;
pair<int, int> moves[256];
bool coord_ok(int i, int j) { return i >= 0 && j >= 0 && i < n && j < p; }
pair<int, int> xy_to_ij(int x, int y) { return {n-1-y, x}; }
pair<int, int> ij_to_xy(int i, int j) { return {j, n-1-i}; }
pair<int, int> dxy_to_dij(int x, int y) { return {-y, x}; }
pair<int, int> dij_to_dxy(int i, int j) { return {j, -i}; }
int boti, botj;
vector<pair<int, int>> neighbors[256];
vector<vector<bool>> visited;

bool _should_move(int i, int j, int di, int dj);
void _move(int i, int j, int di, int dj);
void reset_visited();

bool should_move(int i, int j, int di, int dj) {
    reset_visited(); return _should_move(i, j, di, dj);
}

void move(int i, int j, int di, int dj) {
    reset_visited(); _move(i, j, di, dj);
}

uint64_t solve()
{
    for (int k = 0; k < instructions.size(); k++) {
        auto [dx, dy] = moves[instructions[k]];
        auto [di, dj] = dxy_to_dij(dx, dy);
        if (should_move(boti, botj, di, dj)) {
            move(boti, botj, di, dj);
            boti = boti+di;
            botj = botj+dj;
        }
    }
    uint64_t res = 0;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < p; j++) {
            if (map[i][j] == 'O' || map[i][j] == '[')
                res += 100*i+j;
        }
    }
    return res;
}

int main(int argc, char** argv)
{
    assert(argc==2);
    ifstream file{argv[1]};
    string line;
    while (getline(file, line)) {
        if (line.size() <= 1) break;
        if (line.back() == '\r') line.pop_back();
        for (int j = 0; j < line.size(); j++) {
            if (line[j] == '@') {
                boti = map.size();
                botj = j;
            }
        }
        map.push_back(line);
    }
    for (char c; file>>c;)
        if (c != '\r') instructions.push_back(c);

    n = map.size(); p = map[0].size();
    int boti_cpy = boti, botj_cpy = botj;

    vector<string> map_part2;
    for (int i = 0; i < n; i++) {
        map_part2.push_back({});
        for (int j = 0; j < p; j++) {
            switch (map[i][j]) {
                case 'O':
                    map_part2.back().push_back('[');
                    map_part2.back().push_back(']');
                    break;
                case '@':
                    map_part2.back().push_back('@');
                    map_part2.back().push_back('.');
                    break;
                default :
                    map_part2.back().push_back(map[i][j]);
                    map_part2.back().push_back(map[i][j]);
            }
        }
    }

    moves['^'] = {0, 1};
    moves['v'] = {0, -1};
    moves['>'] = {1, 0};
    moves['<'] = {-1, 0};

    neighbors['@'] = {{0, 0}};
    neighbors['O'] = {{0, 0}};
    neighbors['['] = {{0, 0}, {0, 1}};
    neighbors[']'] = {{0, 0}, {0, -1}};

    cout << "part1: " << solve() << endl;

    map = map_part2; p*=2;
    boti = boti_cpy; botj = botj_cpy*2;

    cout << "part2: " << solve() << endl;

    return 0;
}

bool _should_move(int i, int j, int di, int dj) {
    for (auto [ni, nj] : neighbors[map[i][j]]) {
        if (visited[i+ni][j+nj]) continue;
        visited[i+ni][j+nj] = true;
        if (!coord_ok(i+ni+di, j+nj+dj) || map[i+ni+di][j+nj+dj] == '#') return false;
        if (map[i+ni+di][j+nj+dj] == '.') continue;
        if (!_should_move(i+ni+di, j+nj+dj, di, dj)) return false;
    }

    return true;
}

void _move(int i, int j, int di, int dj) {
    if (map[i][j] == '.') return;
    for (auto [ni, nj] : neighbors[map[i][j]]) {
        if (visited[i+ni][j+nj]) continue;
        visited[i+ni][j+nj] = true;
        _move(i+ni+di, j+nj+dj, di, dj);
        map[i+ni+di][j+nj+dj] = map[i+ni][j+nj];
        map[i+ni][j+nj] = '.';
    }
}

void reset_visited() {
    if (!visited.size()) {
        visited = vector<vector<bool>>(n, vector<bool>(p, false));
    } else {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < p; j++) visited[i][j] = false;
        }
    }
}
