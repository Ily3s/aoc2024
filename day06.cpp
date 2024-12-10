#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cassert>
#include <tuple>
#include <vector>
#include <algorithm>
#include <tuple>

using namespace std;

int main(int argc, char** argv)
{
    assert(argc==2);
    ifstream file{argv[1]};
    vector<string> map;
    vector<vector<bool>>visited;
    vector<vector<bool>> vis4[4];
    int x, y;
    int rot;
    int64_t output = 0;

    tuple<int, int> dirs[] = {
        {0, 1},
        {1, 0},
        {0, -1},
        {-1, 0}
    };

    char assoc[256];
    assoc['^'] = 0;
    assoc['>'] = 1;
    assoc['v'] = 2;
    assoc['<'] = 3;

    while(map.push_back(""), visited.push_back({}), getline(file, map.back())) {
        for (int i = 0; i < map.back().size(); i++) visited.back().push_back(false);
        for (int i = 0; i < map.back().size(); i++) {
            char c = map.back()[i];
            if (c != '#' && c != '.') {x = i; y = map.size()-1; rot = assoc[c];}
        }

    }
    map.pop_back();
    for (int i = 0; i < 4; i++) vis4[i] = visited;


    int n = map.size(), p = map[0].size();
    y = n-1 - y;
    int xcpy = x, ycpy = y, rotcpy = rot;

    auto coord_ok = [n,p] (int a, int b) {return a >= 0 && b >= 0 && a < n && b < p;};

    bool part2 = false;

    int i, j;
    auto visitedcpy = visited;

part1:
    while (true) {
        output += !part2*!visited[n-1-y][x];
        visited[n-1-y][x] = true;
        if (vis4[rot][n-1-y][x]) goto super;
        vis4[rot][n-1-y][x] = true;
        auto [dx, dy] = dirs[rot];
        if (!coord_ok(n-1-(y+dy), x+dx)) break;
        if (map[n-1-(y+dy)][x+dx] == '#') rot = (rot+1)%4;
        else { x += dx; y += dy; }
    }
    if (part2) goto not_super;

    std::cout << "part1: " << output << std::endl;

    part2 = true;
    output = 0;
    visitedcpy = visited;

    for (i = 0; i < n; i++) {
        for (j = 0; j < p; j++) {
            if (visitedcpy[i][j] && (i != (n-1)-ycpy || j != xcpy)) {
                x = xcpy; y = ycpy; rot = rotcpy;
                map[i][j] = '#';
                for(int a=0;a<4;a++){for(int b=0;b<n;b++){for(int c=0;c<p;c++) vis4[a][b][c] = false;}}
                for(int b=0;b<n;b++){for(int c=0;c<p;c++) visited[b][c] = false;}
                goto part1;
super :
                output++;
not_super :
                map[i][j] = '.';
            }
        }
    }

    std::cout << "part2: " << output << std::endl;

    return 0;
}
