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
int n, p;

struct node{
    int data[4]{ -1, -1, -1, -1 };
    int& operator[](size_t i) { return data[i]; }
};
vector<vector<node>> owners;

enum ROTA{
    NORTH=0,
    EAST=1,
    SOUTH=2,
    WEST=3
};

struct element{
    int i, j;
    ROTA rota;
    int64_t priority;
};
vector<element> queue;

pair<int, int> rota2transla[4] = {
    /*[NORTH] =*/ {-1, 0},
    /*[EAST] = */{0, 1},
    /*[SOUTH] =*/ {1, 0},
    /*[WEST] = */{0, -1}
};

bool coord_ok(int i, int j) { return i >= 0 && j >= 0 && i < n && j < p; }

void consider(int i, int j, ROTA rota, int64_t priority, int k) {
    if (!coord_ok(i, j) || map[i][j] == '#') return;
    int for_own = owners[i][j][rota];
    if (!(for_own != -1 && queue[for_own].priority <= priority)) {
        if (for_own != -1) queue[for_own] = {.i=i, .j=j, .rota=rota, .priority=priority+(for_own>k)};
        else {
            queue.push_back({.i=i, .j=j, .rota=rota, .priority=priority});
            owners[i][j][rota] = queue.size()-1;
        }
    }
}


int64_t part1()
{
    owners = vector<vector<node>>(n, vector<node>(p, node()));
    queue.push_back({.i=n-2, .j=1, .rota=ROTA::EAST, .priority=0});
    owners[n-2][1][ROTA::EAST] = 0;
    int64_t output = 0;
    bool control = true;
    while (control) {
        int len = queue.size();
        for (int k = 0; k < len; k++) {
            auto& e = queue[k];
            assert(owners[e.i][e.j][e.rota] == k);
            e.priority--;
            if (e.priority != -1) continue;
            auto [di, dj] = rota2transla[e.rota];
            if (coord_ok(e.i+di, e.j+dj) && map[e.i+di][e.j+dj] == 'E')
                control = false;
            consider(e.i+di, e.j+dj, e.rota, 0, k);
            consider(queue[k].i, queue[k].j, ROTA((queue[k].rota+1)%4), 999, k);
            consider(queue[k].i, queue[k].j, ROTA(queue[k].rota-1+4*(queue[k].rota==0)), 999, k);
        }
        output++;
    }

    return output;
}

vector<vector<bool>> nice_seats;
vector<bool> visited;

void build_path(int i, int j, ROTA rot, int64_t cost, int64_t dcost, int64_t outp1) {
    if (!coord_ok(i, j) || map[i][j] == '#') return;
    if (owners[i][j][rot] == -1 || visited[owners[i][j][rot]]) return;
    if (queue[owners[i][j][rot]].priority != -(cost+dcost)) return;
    cost += dcost;
    visited[owners[i][j][rot]] = true;
    assert(cost <= outp1);
    nice_seats[i][j] = true;
    if (cost != outp1) {
        auto [di, dj] = rota2transla[rot];
        build_path(i-di, j-dj, rot, cost, 1, outp1);
        build_path(i, j, ROTA(rot-1+4*(rot==0)), cost, 1000, outp1);
        build_path(i, j, ROTA((rot+1)%4), cost, 1000, outp1);
    }
}

int64_t part2(int64_t outp1)
{
    nice_seats = vector<vector<bool>>(n, vector<bool>(p, false));
    visited = vector<bool>(queue.size(), false);
    for (int rot = 0; rot < 4; rot++)
        build_path(1, p-2, ROTA(rot), 0, 0, outp1);

    int64_t output = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < p; j++)
            output += nice_seats[i][j];
    }

    return output;
}


int main(int argc, char** argv)
{
    assert(argc==2);
    ifstream file{argv[1]};
    string line;
    while (getline(file, line)) {
        if (line.size() <= 1) break;
        if (line.back() == '\r') line.pop_back();
        map.push_back(line);
    }
    n = map.size(); p = map[0].size();

    int64_t outp1 = part1();
    cout << "part1: " << outp1 << endl;
    cout << "part2: " << part2(outp1) << endl;

    return 0;
}
