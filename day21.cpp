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

struct Pad
{
    pair<int, int> char2coord[256];
    char coord2char[4][3];

    Pad(char coord2char[4][3]);
};

// zeroes are "forbidden" emplacements
char raw_numpad[][3]{
    {'7', '8', '9'},
    {'4', '5', '6'},
    {'1', '2', '3'},
    {0, '0', 'A'}
};
char raw_dirpad[][3]{
    {0, '^', 'A'},
    {'<', 'v', '>'},
    {0, 0, 0},
    {0, 0, 0}
};

Pad numpad{raw_numpad};
Pad dirpad{raw_dirpad};

// compute the cost to go from a to b and press the button
int64_t compute_cost(char a, char b, int k, Pad& pad);

// mem[a][b][k] store the result of compute_cost(a, b, k, ..)
int64_t mem[256][256][26];

int64_t solve(int bot_number)
{
    // set/reset mem
    for (int k = 256*256*26; k--;) ((int64_t*)mem)[k] = -1;

    int64_t output = 0;

    for (string& code : codes) {
        int64_t cost = compute_cost('A', code[0], bot_number, numpad);
        for (int i = 1; i < code.size(); i++)
            cost += compute_cost(code[i-1], code[i], bot_number, numpad);
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

    cout << "part1: " << solve(2) << endl;
    cout << "part2: " << solve(25) << endl;

    return 0;
}

Pad::Pad(char coord2char[4][3]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            this->coord2char[i][j] = coord2char[i][j];
            if (coord2char[i][j])
                char2coord[coord2char[i][j]] = {i, j};
        }
    }
}

void update_coords(int* i, int* j, char c) {
    switch(c) {
        case '<': (*j)--; break;
        case '>': (*j)++; break;
        case 'v': (*i)++; break;
        case '^': (*i)--; break;
    }
}

int64_t compute_cost(char a, char b, int k, Pad& pad)
{
    auto [ia, ja] = pad.char2coord[a];
    auto [ib, jb] = pad.char2coord[b];

    if (!k) return abs(ia-ib)+abs(ja-jb)+1;
    if (mem[a][b][k] != -1) return mem[a][b][k];
    if (a == b) return 1;

    int chari = ia > ib ? '^' : 'v';
    int charj = ja > jb ? '<' : '>';
    int di = abs(ia-ib), dj = abs(ja-jb);

    // build all legal set of moves that moves the bot's arm
    // from a to b directly (without going backwards or anything)
    vector<string> possibilities;
    string poss(di+dj, 0);
    for (int k = 0; k < 1<<(di+dj); k++) {
        int nbi = 0, x = ja, y = ia;
        bool forbiden = false;
        for (int i = 0; i < di+dj; i++) {
            poss[i] = k&(1<<i) ? chari : charj;
            update_coords(&y, &x, poss[i]);
            forbiden |= !pad.coord2char[y][x];
            nbi += !!(k&(1<<i));
        }
        if (nbi == di && !forbiden) possibilities.push_back(poss);
    }

    // recursively compute the cost of each of these possibilities
    auto results = vector<int64_t>(possibilities.size(), 0);
    for (int i = 0; i < possibilities.size(); i++) {
        results[i] += compute_cost('A', possibilities[i][0], k-1, dirpad);
        for (int j = 1; j < possibilities[i].size(); j++)
            results[i] += compute_cost(possibilities[i][j-1], possibilities[i][j], k-1, dirpad);
        results[i] += compute_cost(possibilities[i].back(), 'A', k-1, dirpad);
    }

    // result is the minimum
    auto it = min_element(results.begin(), results.end());
    mem[a][b][k] = *it;
    return mem[a][b][k];
}
