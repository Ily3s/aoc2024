#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <cstdint>

using namespace std;

int main(int argc, char** argv)
{
    assert(argc==2);
    ifstream file{argv[1]};
    string line;

    uint64_t output[2] = {0, 0};

    while (getline(file, line)) {
        int64_t x[3], y[3];
        for (int i = 0; i < 3; i++) {
            char* ptr = &line[12-(i==2)*3];
            x[i] = strtoll(ptr, &ptr, 10);
            y[i] = strtoll(ptr+4, NULL, 10);
            getline(file, line);
        }

        for (int i = 0; i < 2; i++) {
            if (y[0] && (y[0]*x[1]-y[1])) { // edge cases never happen in my input
                if ((y[0]*x[2]-y[2]*x[0])%(y[0]*x[1]-y[1]*x[0]) == 0) {
                    int64_t lamb2 = (y[0]*x[2]-y[2]*x[0])/(y[0]*x[1]-y[1]*x[0]);
                    if ((y[2]-lamb2*y[1])%y[0] == 0) {
                        int64_t lamb1 = (y[2]-lamb2*y[1])/y[0];
                        output[i] += lamb1*3+lamb2;
                    }
                }
            }
            x[2] += 10000000000000;
            y[2] += 10000000000000;
        }
    }

    cout << "part1: " << output[0] << endl;
    cout << "part2: " << output[1] << endl;

    return 0;
}
