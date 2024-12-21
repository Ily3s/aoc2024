#include <climits>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <cassert>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

vector<string> patterns;
vector<string> designs;

bool parteq(const char* s1, const char* s2) {
    return !*s1 || !*s2 || (*s1==*s2)&&parteq(s1+1, s2+1);
}

// the number of ways to build the kth design, from index i to the end
uint64_t _nbway(int k, int i, vector<int64_t>& mem) {
    if (i == designs[k].size()) return 1;
    if (i > designs[k].size()) return 0;
    if (mem[i] == -1) return 0;
    if (mem[i]) return mem[i];
    for (int j = 0; j < patterns.size(); j++) {
        if (parteq(&designs[k][i], &patterns[j][0]))
            mem[i] += _nbway(k, i+patterns[j].size(), mem);
    }
    if (!mem[i]) {
        mem[i] = -1;
        return 0;
    }
    return mem[i];
}

uint64_t nbway(int k) {
    auto mem = vector<int64_t>(designs[k].size(), 0);
    return _nbway(k, 0, mem);
}

int main(int argc, char** argv)
{
    assert(argc==2);
    ifstream file{argv[1]};
    string line;
    getline(file, line);
    stringstream sstream{line};

    while(getline(sstream, line, ',')) {
        if (line.back() == '\r') line.pop_back();
        if (line.front() == ' ') line.erase(0, 1);
        patterns.push_back(line);
    }
    getline(file, line);
    while (getline(file, line)) {
        if (!line.size()) break;
        if (line.back() == '\r') line.pop_back();
        designs.push_back(line);
    }

    uint64_t output1 = 0, output2 = 0;

    for (int k = designs.size(); k--;) {
        uint64_t res = nbway(k);
        output1 += !!res;
        output2 += res;
    }

    cout << "part1: " << output1 << '\n' << "part2: " << output2 << endl;

    return 0;
}
