#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cassert>

using namespace std;

int main(int argc, char** argv)
{
    assert(argc==2);
    ifstream file{argv[1]};
    file.seekg(0, ios::end);
    int size = file.tellg();
    file.seekg(0, ios::beg);
    std::string dat(size, 0);
    file.read(&dat[0], size);

    int64_t output = 0, output2 = 0;
    bool enable = true;

    for (int i = 0; i < dat.size(); i++) {
        if (i < dat.size()-4 && dat.substr(i, 4) == "do()") {
            enable = true; i += 3; continue;
        }
        if (i < dat.size()-7 && dat.substr(i, 7) == "don't()") {
            enable = false; i += 6; continue;
        }
        if (i < dat.size()-5 && dat.substr(i, 4) == "mul(") {
            if (dat[i+4] > '9' || dat[i+4] < '0') continue;
            char* end;
            int a = strtol(&dat[i+4], &end, 10);
            if (end+1-&dat[0] >= dat.size() || *end != ',' || *(end+1) > '9' || *(end+1) < '0') continue;
            int b = strtol(end+1, &end, 10);
            if (end-&dat[0] >= dat.size() || *end != ')') continue;
            output += a*b;
            if (enable) output2 += a*b;
            i = end-&dat[0];
        }
    }

    std::cout << "part1: " << output << std::endl;
    std::cout << "part2: " << output2 << std::endl;

    return 0;
}
