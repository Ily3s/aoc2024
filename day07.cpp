#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>

using namespace std;

vector<vector<uint64_t>> numbers;
vector<uint64_t> test_vals;

int bruteforce(int i, int j, uint64_t val) {
    if (j == numbers[i].size()) return val == test_vals[i];
    else return bruteforce(i, j+1, val+numbers[i][j]) + bruteforce(i, j+1, val*numbers[i][j]);
}

uint64_t fastpow(int x, int n) {
    if (n == 0) return 1;
    if (n == 1) return x;
    if (n%2 == 0) return fastpow(x*x, n/2);
    return x*fastpow(x*x, n/2);
}

int bruteforce2(int i, int j, uint64_t val) {
    if (j == numbers[i].size()) return val == test_vals[i];
    else return bruteforce2(i, j+1, val+numbers[i][j]) + bruteforce2(i, j+1, val*numbers[i][j])
        + bruteforce2(i, j+1, val*fastpow(10, (int)(log10(numbers[i][j]))+1)+numbers[i][j]);
}

int main(int argc, char** argv)
{
    assert(argc==2);
    ifstream file{argv[1]};
    while (test_vals.push_back(0), file >> test_vals.back()) {
        file.get(); numbers.emplace_back(); char c;
        file.get(c); assert(c == ' ');
        do {
            numbers.back().push_back(0);
            file >> numbers.back().back();
            file.get(c);
        } while (c == ' ');
    }
    test_vals.pop_back();

    uint64_t output = 0, output2 = 0;
    for (int i = 0; i < numbers.size(); i++) {
        output += test_vals[i]*(bruteforce(i, 1, numbers[i][0]) != 0);
        output2 += test_vals[i]*(bruteforce2(i, 1, numbers[i][0]) != 0);
    }

    std::cout << "part1: " << output << std::endl;
    std::cout << "part2: " << output2 << std::endl;

    return 0;
}
