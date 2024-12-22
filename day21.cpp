#include <algorithm>
#include <climits>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <vector>

using namespace std;

string codes[5];

// char -> coordinates(x, y)
pair<int, int> numpad[256];
pair<int, int> dirpad[256];
int64_t mem[256][256][26];

int64_t cost_dir(char a, char b, int k) {
    auto [ia, ja] = dirpad[a]; auto [ib, jb] = dirpad[b];
    if (k==1) return abs(ia-ib)+abs(ja-jb)+1;
    if (mem[a][b][k] != -1) return mem[a][b][k];
    int chari = ia > ib ? '^' : 'v';
    int charj = ja > jb ? '<' : '>';
    int di = abs(ia-ib), dj = abs(ja-jb);
    int ii = 0, ij = 0;
    int64_t cost1 = 0, cost2 = 0;
    for (;ii<di; ii++) {
        if (ia==1 && ja==0 && !ii) {
            cost1 += cost_dir('A', charj, k-1);
            cost1 += cost_dir(charj, chari, k-1);
            ij++;
            continue;
        }
        cost1 += cost_dir(ii?chari:'A', chari, k-1);
    }
    int v = ij;
    for (;ij<dj; ij++) {
        cost1 += cost_dir((ij!=v)?charj:di?chari:'A', charj, k-1);
    }
    cost1 += cost_dir(v<dj?charj:di?chari:'A', 'A', k-1);
    ii = ij = 0;
    for (;ij<dj; ij++) {
        if (ia == 0 && jb==0 && ja-(ij+1) == 0) {
            cost2 += cost_dir(ij?charj:'A', chari, k-1);
            cost2 += cost_dir(chari, charj, k-1);
            ii++;
            continue;
        }
        cost2 += cost_dir(ij?charj:'A', charj, k-1);
    }
    v = ii;
    for (;ii<di; ii++) {
        cost2 += cost_dir((ii!=v)?chari:dj?charj:'A', chari, k-1);
    }
    cost2 += cost_dir(v<di?chari:dj?charj:'A', 'A', k-1);
    mem[a][b][k] = min(cost1, cost2);
    return mem[a][b][k];
}

int64_t cost_num(char a, char b, int robot_number) {
    if (a==b) return 1;
    auto [ia, ja] = numpad[a]; auto [ib, jb] = numpad[b];
    int chari = ia > ib ? '^' : 'v';
    int charj = ja > jb ? '<' : '>';
    int di = abs(ia-ib), dj = abs(ja-jb);
    vector<string> possibilities;
    char* poss = new char[di+dj+1];
    poss[di+dj] = '\0';
    for (int k = 0; k < 1<<(di+dj); k++) {
        int nbi = 0, x = ja, y = ia;
        bool forbiden = false;
        for (int i = 0; i < di+dj; i++) {
            poss[i] = k&(1<<i) ? chari : charj;
            if (k&(1<<i))
                y += ia<ib ? 1 : -1;
            else
                x += ja<jb ? 1 : -1;
            if (x == 0 && y == 3) forbiden = true;
            nbi += !!(k&(1<<i));
        }
        if (nbi == di && !forbiden) possibilities.push_back(poss);
    }
    auto res = vector<int64_t>(possibilities.size(), 0);
    for (int i = 0; i < possibilities.size(); i++) {
        res[i] += cost_dir('A', possibilities[i][0], robot_number);
        for (int j = 1; j < possibilities[i].size(); j++)
            res[i] += cost_dir(possibilities[i][j-1], possibilities[i][j], robot_number);
        res[i] += cost_dir(possibilities[i].back(), 'A', robot_number);
    }
    auto it = min_element(res.begin(), res.end());

    delete[] poss;
    return *it;
}

int64_t solve(int bot_number) {
    for (int k = 0; k < 26; k++) {
        for (int i = 256; i--;) {
            for (int j = 256; j--;) mem[i][j][k] = -1;
        }
    }

    int64_t output = 0;

    for (string& code : codes) {
        int64_t cost = cost_num('A', code[0], bot_number);
        for (int i = 1; i < code.size(); i++)
            cost += cost_num(code[i-1], code[i], bot_number);
        cost *= strtoll(code.c_str(), NULL, 10);
        output += cost;
    }
    
    return output;
}


int main(int argc, char** argv)
{
    // input parsing
    assert(argc==2);
    ifstream file{argv[1]};
    for (int i = 0; i<5 && getline(file, codes[i]); i++)
        if (codes[i].back() == '\r') codes[i].pop_back();

    // init numpad and dirpad
    numpad['0'] = {3, 1};
    numpad['A'] = {3, 2};
    for (int i = 1; i <= 9; i++)
        numpad['0'+i] = {2-(i-1)/3, (i-1)%3};
    dirpad['A'] = {0, 2};
    dirpad['^'] = {0, 1};
    dirpad['<'] = {1, 0};
    dirpad['v'] = {1, 1};
    dirpad['>'] = {1, 2};

    cout << "part1: " << solve(2) << endl;
    cout << "part2: " << solve(25) << endl;

    return 0;
}
