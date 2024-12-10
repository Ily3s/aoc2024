#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cassert>
#include <tuple>
#include <vector>

using namespace std;

int main(int argc, char** argv)
{
    assert(argc==2);
    ifstream file{argv[1]};
    vector<string> grid;
    while (grid.push_back(""), getline(file, grid.back()));
    grid.pop_back();

    int64_t output = 0, output2 = 0;

    string word = "XMAS";
    std::tuple<int, int> directors[] = {
        {1, 0}, {-1, 0},
        {0, 1}, {0, -1},
        {1, 1}, {-1, 1}, {1, -1}, {-1, -1}
    };
    int n = grid.size(), p = grid[0].size();
    auto coord_ok = [n,p] (int a, int b) {return a >= 0 && b >= 0 && a < n && b < p;};

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < p; j++) {
            for (auto [r, s] : directors) {
                bool word_found = true;
                int a = i, b = j;
                for (int k = 0; k < word.size(); k++) {
                    if (!coord_ok(a, b) || grid[a][b] != word[k]) {
                        word_found = false;
                        break;
                    }
                    a += r; b += s;
                }
                output += word_found;
            }

            if (grid[i][j] == 'A' && coord_ok(i-1, j-1) && coord_ok(i+1, j+1)) {
                int corners[] = {grid[i-1][j-1], grid[i+1][j-1], grid[i-1][j+1], grid[i+1][j+1]};
                int Mc = 0, Sc = 0;
                for (int k = 0; k < 4; k++) {Mc += corners[k]=='M'; Sc += corners[k]=='S';}
                output2 += Mc == 2 && Sc == 2 && grid[i-1][j-1] != grid[i+1][j+1];
            }
        }
    }

    std::cout << "part1: " << output << std::endl;
    std::cout << "part2: " << output2 << std::endl;

    return 0;
}
