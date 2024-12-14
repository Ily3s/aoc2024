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

#define N 103
#define P 101

struct robot {
    int x, y, dx, dy;
};

float variance_x(const vector<robot>& bots) {
    float mean = 0;
    for (auto& bot : bots) mean += bot.x;
    mean /= bots.size();
    float var = 0;
    for (auto& bot : bots) var += abs(mean-bot.x)*abs(mean-bot.x);
    return var;
}

float variance_y(const vector<robot>& bots) {
    float mean = 0;
    for (auto& bot : bots) mean += bot.y;
    mean /= bots.size();
    float var = 0;
    for (auto& bot : bots) var += abs(mean-bot.y)*abs(mean-bot.y);
    return var;
}

int main(int argc, char** argv)
{
    assert(argc==2);
    ifstream file{argv[1]};
    string line;

    vector<robot> robots;
    int t = 100;
    int quadrants[4]{}; // number of robots in each quadrant
    while (getline(file, line)) {
        // parsing
        int dat[4];
        char* ptr = &line[2];
        for (int i = 0; i < 4; i++) {
            dat[i] = strtol(ptr, &ptr, 10);
            ptr += 1+2*(i==1);
        }
        // for part2
        robots.push_back({dat[0], dat[1], dat[2], dat[3]});
        // actual part 1
        int x = (dat[0]+t*dat[2])%P;
        int y = (dat[1]+t*dat[3])%N;
        if (x < 0) x += P;
        if (y < 0) y += N;
        quadrants[(x<P/2)*2+(y<N/2)] += x != P/2 && y != N/2; // trust me
    }

    int64_t output = 1;
    for (int i = 0; i < 4; i++) output *= quadrants[i];
    cout << "part1: " << output << endl;

    int X = 0, Y = 0; // X and Y are the times where thick lines appear in the middle
                      // they are detected automatically by calculating variance of positions
    float min_var_x = INFINITY, min_var_y = INFINITY;

    for (int i = 0; i < N; i++) {
        int var_x = variance_x(robots);
        int var_y = variance_y(robots);
        if (i < P && var_x < min_var_x) {
            min_var_x = var_x;
            X = i;
        }
        if (var_y < min_var_y) {
            min_var_y = var_y;
            Y = i;
        }

        for (auto& bot : robots) {
            int x = (bot.x+bot.dx)%P;
            int y = (bot.y+bot.dy)%N;
            if (x < 0) x += P;
            if (y < 0) y += N;
            bot.x = x; bot.y = y;
        }
    }

    // The rest of it is just arithmetics i guess
    int a = 0, b = 0; // should have at the end P*a-N*b = Y-X
    while (a*P%(N-P) != abs(Y-X)%(N-P) || a*P < (Y-X)) a++;
    b = (a*P-(Y-X))/(N-P); a += b;

    cout << "part2: " << X+P*a << endl;

    // I'm not printing the tree in the terminal to have the same format of output in all my days

    return 0;
}
