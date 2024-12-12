#include <climits>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cassert>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;

struct node{
    uint64_t val;
    node* next;
};

void insert(node* l, uint64_t val) {
    node* next = new node;
    next->val = val;
    next->next = l->next;
    l->next = next;
}

void print_l(node* l) {
    if (!l) {cout << endl; return;}
    cout << l->val << " ";
    print_l(l->next);
}

uint64_t fastpow(uint64_t x, int n) {
    if (n == 0) return 1;
    if (n == 1) return x;
    if (n%2 == 0) return fastpow(x*x, n/2);
    return x*fastpow(x*x, n/2);
}

struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2>& p) const
    {
        // Hash the first element
        size_t hash1 = hash<T1>{}(p.first);
        // Hash the second element
        size_t hash2 = hash<T2>{}(p.second);
        // Combine the two hash values
        return hash1
               ^ (hash2 + 0x9e3779b9 + (hash1 << 6)
                  + (hash1 >> 2));
    }
};

unordered_map<pair<uint64_t, int>, uint64_t, hash_pair> results{};

int depth_max = 25;
uint64_t solve(uint64_t stone, int depth) {
    if (depth == depth_max) return 1;
    auto it = results.find({stone, depth});
    if (it != results.end()) return it->second;
    uint64_t res = 0;
    int digits = (int)(log10(stone)) + 1;
    if (stone == 0) res = solve(1, depth+1);
    else if (digits%2 == 0) {
        uint64_t tenpo = fastpow(10, digits/2);
        res += solve(stone/tenpo, depth+1);
        res += solve(stone%tenpo, depth+1);
    } else {
        res += solve(stone*2024, depth+1);
    }
    results.insert({{stone, depth}, res});
    return res;
}

int main(int argc, char** argv)
{
    assert(argc==2);
    ifstream file{argv[1]};
    node* list = new node;
    int val;
    for (node* curr = list; file>>val; curr = curr->next) insert(curr, val);
    list = list->next;

    uint64_t output1 = 0;
    depth_max = 25;
    for (node* curr = list; curr != nullptr; curr = curr->next) output1 += solve(curr->val, 0);
    cout << "part1: " << output1 << endl;

    depth_max = 75;
    results.clear();
    uint64_t output2 = 0;
    for (node* curr = list; curr != nullptr; curr = curr->next) output2 += solve(curr->val, 0);
    cout << "part2: " << output2 << endl;

    return 0;
}
