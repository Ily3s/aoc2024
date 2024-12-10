#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cassert>
#include <sstream>
#include <vector>

using namespace std;

int main(int argc, char** argv)
{
    assert(argc==2);
    ifstream file{argv[1]};

    std::string line;
    int output1 = 0, output2 = 0;
    while (getline(file, line)) {
        bool b[2][2] = {{true, true}, {true, true}};
        stringstream ss{line}; vector<int> levels;
        for (int i = 0; levels.push_back(0), ss>>levels[i]; i++);
        levels.pop_back();
        for (int i = 0; i < 2; i++) {
            bool tolerating = true;
            for (int j = 1; j < levels.size(); j++) {
                auto is_ok = [&levels](int a, int b) { return levels[b] > levels[a] && levels[b]-3 <= levels[a]; };
                b[0][i] &= is_ok(j-1, j);
                b[1][i] &= is_ok(j-1, j);
                if (!b[1][i] && tolerating) {
                    tolerating = false;
                    if (j == levels.size()-1) {b[1][i] = true; continue;}
                    b[1][i] =
                        (is_ok(j, j+1) && (j==1 || is_ok(j-2, j))) ||
                        is_ok(j-1, j+1);
                    j++;
                }
            }
            levels = {levels.rbegin(), levels.rend()};
        }
        output1 += b[0][0] || b[0][1];
        output2 += b[1][0] || b[1][1];
    }
    std::cout << "part1: " << output1 << std::endl;
    std::cout << "part2: " << output2 << std::endl;

    return 0;
}
