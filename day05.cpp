#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cassert>
#include <tuple>
#include <vector>
#include <algorithm>

using namespace std;

int main(int argc, char** argv)
{
    assert(argc==2);
    ifstream file{argv[1]};
    vector<vector<int>> updates;
    vector<tuple<int, int>> ordering;
    string line; bool banana = true;
    while (getline(file, line)) {
        if (line.size()<=2) {banana = false; continue;}
        if (banana) {
            ordering.push_back({atoi(&line[0]), atoi(&line[3])});
        } else {
            char* p = &line[0]-1; updates.push_back({});
            do {
                updates.back().push_back(strtol(p+1, &p, 10));
            } while (*p == ',');
        }
    }

    int64_t output = 0, output2 = 0;

    for (int k = 0; k < updates.size(); k++) {
        bool is_correct = true;
        vector<vector<int>> g(updates[k].size());
        vector<bool> is_min(updates[k].size(), true);
        for (int i = 0; i < ordering.size(); i++) {
            auto [a,b] = ordering[i];
            auto ita = std::find(updates[k].begin(), updates[k].end(), a);
            auto itb = std::find(updates[k].begin(), updates[k].end(), b);
            if (ita != updates[k].end() && itb != updates[k].end() && ita > itb)
                is_correct = false;
            if (ita != updates[k].end() && itb != updates[k].end()) {
                g[ita-updates[k].begin()].push_back(itb-updates[k].begin());
                is_min[itb-updates[k].begin()] = false;
            }
        }
        if (is_correct) output += updates[k][updates[k].size()/2];
        else {
            vector<int> path{};
            vector<int> usefull{};
            int curr = 0;
            for (int i = 0; i < updates[k].size(); i++) curr += i*is_min[i];

            while (path.size() != updates[k].size()) {
                if (path.size() && path.back() == curr)
                    usefull.back()++;
                else {
                    path.push_back(curr);
                    if (path.size() == updates[k].size()) break;
                    usefull.push_back(0);
                }
                if (usefull.back() >= g[curr].size()) {
                    path.pop_back();
                    usefull.pop_back();
                    curr = path.back();
                    continue;
                }
                curr = g[curr][usefull.back()];
            }

            output2 += updates[k][path[path.size()/2]];
        }
    }

    std::cout << "part1: " << output << std::endl;
    std::cout << "part2: " << output2 << std::endl;

    return 0;
}
