#include <array>
#include <climits>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cassert>
#include <sstream>
#include <vector>
#include <cstdint>

using namespace std;

uint64_t regs[256];
uint64_t &A = regs['A'], &B = regs['B'], &C = regs['C'];
vector<int> program{};
int ptr = 0;
vector<int> outputs;

uint64_t combo(int operand) {
    if (operand == 7) throw;
    if (operand <= 3) return operand;
    else return regs['A'+(operand-4)];
}

vector<int> As;

bool solve(int i, uint64_t Alr) {
    if (i==program.size()) return true;
    for (int k = 0; k <= 8; k++) {
        ptr = 0;
        A = Alr;
        uint64_t Al = Alr*8+k;
        if ((5^k^(Al>>(k^1)))%8 == program[program.size()-1-i]) {
            As.push_back(k);
            if (solve(i+1, Al)) return true;
            else As.pop_back();
        }
    }
    return false;
}

int main(int argc, char** argv)
{
    assert(argc==2);
    ifstream file{argv[1]};
    string line;
    for (char c = 'A'; c <= 'C'; c++) {
        getline(file, line);
        regs[c] = strtoll(&line[12], NULL, 10);
    }
    getline(file, line);
    file >> line;
    int nb; char c;
    while (file>>nb) {
        program.push_back(nb);
        file>>c;
    }

    while (ptr < program.size()-1) {
        int opcode = program[ptr];
        int operand = program[ptr+1];
        switch (opcode) {
            case 1: B ^= operand; break;
            case 2: B = combo(operand)%8; break;
            case 3: if(A) {ptr = operand; continue;}
            case 4: B = B^C; break;
            case 5: outputs.push_back(combo(operand)%8); break;
            default: regs['A'+opcode-!!opcode*5] = A>>combo(operand);
        }
        ptr += 2;
    }

    cout << "part1: ";
    for (int i = 0; i < outputs.size(); i++)
        cout << outputs[i] << (i!=outputs.size()-1 ? ',' : '\n');

    solve(0, 0);
    uint64_t output = 0;
    for (int i = 0; i < As.size(); i++) {
        output = 8*output+As[i];
    }
    cout << "part2: " << output << endl;

    return 0;
}
