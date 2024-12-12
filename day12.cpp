#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <cstdint>

using namespace std;

vector<string> map;
vector<vector<int>> components;

pair<int, int> neighbors[]{
    {1, 0},
    {0, 1},
    {-1, 0},
    {0, -1}
};

int n, p;

bool coord_ok(int i, int j) { return i >= 0 && j >= 0 && i < n && j < p; }

struct region{
    int area = 0, perimeter = 0;
    vector<pair<int, int>> frontier[4]{};
};

void conn(int i, int j, char c, int comp, region* ptr, int from) {
    if (!coord_ok(i, j) || map[i][j] != c) {
        ptr->perimeter+=1;
        ptr->frontier[from].emplace_back(i, j);
        return;
    }
    if (components[i][j] == comp) return;
    ptr->area++;
    components[i][j] = comp;
    for (auto [r, s] : neighbors)
        conn(i+r, j+s, c, comp, ptr, r+s==1 ? s : 3+r);
}

int lexicomp(const void* a, const void* b) {
    auto [ia, ja] = *(const pair<int, int>*)a;
    auto [ib, jb] = *(const pair<int, int>*)b;
    if (ia > ib) return 1;
    if (ia < ib) return -1;
    return ja - jb;
}

int main(int argc, char** argv)
{
    assert(argc==2);
    ifstream file{argv[1]};
    while(map.push_back(""), getline(file, map.back()));
    map.pop_back();
    n = map.size(); p = map[0].size();
    components = vector<vector<int>>(n, vector<int>(p, -1));

    int comp = 0;
    uint64_t output1 = 0, output2 = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < p; j++) {
            if (components[i][j] == -1) {
                region reg;
                conn(i, j, map[i][j], comp++, &reg, 0);
                output1 += reg.area * reg.perimeter;
                int sides = 0;
                for (int k = 0; k < 4; k++) {
                    if (k%2 == 1) {
                        for (int u = 0; u < reg.frontier[k].size(); u++) {
                            reg.frontier[k][u].first += reg.frontier[k][u].second;
                            reg.frontier[k][u].second = reg.frontier[k][u].first - reg.frontier[k][u].second;
                            reg.frontier[k][u].first -= reg.frontier[k][u].second;
                        }
                    }
                    qsort(&reg.frontier[k][0], reg.frontier[k].size(), sizeof(pair<int, int>), lexicomp);
                    for (int u = 0; u < reg.frontier[k].size(); u++) {
                        sides++;
                        while (u+1 < reg.frontier[k].size() && reg.frontier[k][u+1].first == reg.frontier[k][u].first &&
                                (reg.frontier[k][u+1].second == reg.frontier[k][u].second ||
                                 reg.frontier[k][u+1].second == reg.frontier[k][u].second + 1))
                            u++;
                    }
                }
                output2 += sides*reg.area;
            }

        }
    }

    cout << "part1: " << output1 << endl;
    cout << "part2: " << output2 << endl;

    return 0;
}
