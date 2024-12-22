#include <algorithm>
#include <climits>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <array>

using namespace std;

vector<int64_t> secret_nbs;
array<int, 19*19*19*19> visited;
array<int, 19*19*19*19> seq2bananas;

int main(int argc, char** argv)
{
    // input parsing
    assert(argc==2);
    ifstream file{argv[1]};
    int64_t sec;
    while (file>>sec) secret_nbs.push_back(sec);

    seq2bananas.fill(0);
    visited.fill(-1);

    // keep track of what needs to be reset in visited,
    // avoids reseting the whole thing everytime
    vector<int> memory;

    int64_t output1 = 0;
    for (int k = 0; k < secret_nbs.size(); k++) {
        int64_t& nb = secret_nbs[k];
        int prev = nb%10;
        // this number uniquely identify a sequence
        int seq = 0;
        for (int i = 0; i<2000; i++) {
            // part1 :
            int64_t s1 = ((nb<<6)^nb)%(1<<24);
            int64_t s2 = ((s1>>5)^s1)%(1<<24);
            nb = ((s2<<11)^s2)%(1<<24);
            // part2:
            int diff = nb%10 - prev;
            seq = (seq%(19*19*19))*19+(diff+9);
            if (i >= 3 && visited[seq] == -1) {
                visited[seq] = nb%10;
                memory.push_back(seq);
                seq2bananas[seq] += nb%10;
            }
            prev = nb%10;
        }
        output1 += nb;
        for (auto i : memory)
            visited[i] = -1;
        memory.clear();
    }

    cout << "part1: " << output1 << endl;
    cout << "part2: " << *max_element(seq2bananas.begin(), seq2bananas.end()) << endl;

    return 0;
}
