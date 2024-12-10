#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>

using namespace std;

int main(int argc, char** argv)
{
    assert(argc==2);
    vector<int> l[2];
    ifstream file{argv[1]};
    int curr;
    for (int i = 0; file>>curr; i=!i) l[i].push_back(curr);
    assert(l[0].size() == l[1].size());

    qsort(&l[0][0], l[0].size(), sizeof(int), [](const void* a, const void* b) {return *(int*)a - *(int*)b;});
    qsort(&l[1][0], l[1].size(), sizeof(int), [](const void* a, const void* b) {return *(int*)a - *(int*)b;});
    for (int i = curr =0; i < l[0].size(); i++) curr += std::abs(l[0][i] - l[1][i]);

    std::cout << "part1: " << curr << std::endl;

    for(int i=curr=0, j=0; i < l[0].size() && j < l[1].size();) {
        int icpy = i, jcpy = j;
        if (l[0][i] < l[1][j]) {i++; continue;}
        if (l[0][i] > l[1][j]) {j++; continue;}
        while (i < l[0].size() && l[0][i]==l[0][icpy]) i++;
        while (j < l[1].size() && l[1][j]==l[1][jcpy]) j++;
        curr += l[0][icpy]*(i-icpy)*(j-jcpy);
    }

    std::cout << "part2: " << curr << std::endl;

    return 0;
}
