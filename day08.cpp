#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cassert>
#include <utility>
#include <vector>

using namespace std;

vector<pair<int, int>> antennas[256]{};
vector<string> map;
vector<vector<bool>> antinodes{};
vector<vector<bool>> antinodes2{};
int output1 = 0, output2 = 0;

int main(int argc, char** argv)
{
    assert(argc==2);
    ifstream file{argv[1]};
    while (map.push_back(""), getline(file, map.back())) {
        antinodes.emplace_back(map.back().size(), false);
        antinodes2.emplace_back(map.back().size(), false);
        for (int i = 0; i < map.back().size(); i++)
            antennas[map.back()[i]].emplace_back(map.size()-1, i);
    }
    map.pop_back();

    int n = map.size(), p = map[0].size();
    auto coord_ok = [n,p] (int a, int b) {return a >= 0 && b >= 0 && a < n && b < p;};

    for (int k = 0; k < 256; k++) {
        if (k == '#' || k == '.') continue;

        for (int i = 0; i < antennas[k].size(); i++) {
            for (int j = i+1; j < antennas[k].size(); j++) {
                auto [y1, x1] = antennas[k][i];
                auto [y2, x2] = antennas[k][j];
                if (coord_ok(2*y2-y1, 2*x2-x1)) {
                    output1 += !antinodes[2*y2-y1][2*x2-x1];
                    antinodes[2*y2-y1][2*x2-x1] = true;
                }
                if (coord_ok(2*y1-y2, 2*x1-x2)) {
                    output1 += !antinodes[2*y1-y2][2*x1-x2];
                    antinodes[2*y1-y2][2*x1-x2] = true;
                }
                auto [dy, dx] = pair<int,int>{y2-y1, x2-x1};
                for (int x = x2, y = y2; coord_ok(y, x); x+=dx, y+=dy) {
                    output2 += !antinodes2[y][x];
                    antinodes2[y][x] = true;
                }
                for (int x = x1, y = y1; coord_ok(y, x); x-=dx, y-=dy) {
                    output2 += !antinodes2[y][x];
                    antinodes2[y][x] = true;
                }
            }
        }
    }

    std::cout << "part1: " << output1 << std::endl;
    std::cout << "part2: " << output2 << std::endl;

    return 0;
}
