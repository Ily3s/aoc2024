#include <climits>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cassert>
#include <utility>
#include <vector>

using namespace std;

vector<int> disk;

int main(int argc, char** argv)
{
    assert(argc==2);
    ifstream file{argv[1]};

    char c;
    for(int i = 0; file >> c; i++) {
        for (int k = 0; k < c - '0'; k++) disk.push_back(i);
        if (!(file>>c)) break;
        for (int k = 0; k < c - '0'; k++) disk.push_back(-1);
    }
    auto disk2 = disk;

    int i = 0, j = disk.size()-1;
    while (i < j) {
        if (disk[j] == -1) {j--; continue;}
        if (disk[i] != -1) {i++; continue;}
        disk[i] = disk[j];
        disk[j] = -1;
    }

    uint64_t output1 = 0;
    for (int i = 0; i < disk.size() && disk[i] != -1; i++) output1 += i*disk[i];
    cout << "part1: " << output1 << endl;

    uint64_t output2 = 0;

    int ID = INT_MAX; j = disk2.size()-1;
    while (disk2[j] == -1) j--;
    ID = disk2[j];
    int max_seen = INT_MAX;
    while (ID > 0) {
        while (disk2[j] != ID) j--;
        int m = j;
        while (m>0 && disk2[m] == ID) m--;
        int c = 0;
        bool found = false;
        int k = 0;
        if (j-m >= max_seen) goto lose;
        for (; k < j; k++) {
            if (disk2[k] != -1) {c = 0; continue;}
            c++;
            if (c == j-m) {found = true; break;}
        }
        if (found) {
            for (int r = k-(c-1); r <= k; r++) disk2[r] = ID; 
            for (int r = m+1; r <= j; r++) disk2[r] = -1;
        } else {
            max_seen = j-m;
        }
lose:
        ID--;
    }

    for (int i = 0; i < disk2.size(); i++) output2 += (disk2[i]!=-1)*i*disk2[i];
    cout << "part2: " << output2 << endl;

    return 0;
}
