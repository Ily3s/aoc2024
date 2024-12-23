#include <climits>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <array>

using namespace std;

struct node{
    vector<node*> neighbors;
    array<bool, 26*26> is_neighb;
};
array<node, 26*26> graph;
array<vector<node*>, 26*26> components;

bool tfirst(node* n) {
    return (n-&graph[0])/26+'a' == 't';
}

void print_node(int nb) {
    cout << (char)(nb/26+'a') << (char)(nb%26+'a');
}

// for part2
vector<int> largest_set;
vector<int> current_set;
void compute(int k) {
    if (k > 26*26) {
        if (current_set.size() > largest_set.size())
            largest_set = current_set;
        return;
    }
    compute(k+1);
    for (int n : current_set) {
        if (!graph[k].is_neighb[n]) return;
    }
    current_set.push_back(k);
    compute(k+1);
    current_set.pop_back();
}

int main(int argc, char** argv)
{
    assert(argc==2);
    ifstream file{argv[1]};
    string line;
    while (getline(file, line)) {
        if (line.size() <= 1) break;
        int a = (line[0]-'a')*26+(line[1]-'a');
        int b = (line[3]-'a')*26+(line[4]-'a');
        graph[a].neighbors.push_back(&graph[b]);
        graph[b].neighbors.push_back(&graph[a]);
        graph[a].is_neighb[b] = true;
        graph[b].is_neighb[a] = true;
    }

    int output1 = 0;

    for (int i = 0; i < 26*26; i++) {
        if (tfirst(&graph[i])) {
            for (node* n1 : graph[i].neighbors) {
                if (n1 <= &graph[i] && tfirst(n1)) continue;
                for (node* n2: graph[i].neighbors) {
                    if (n2 <= n1) continue;
                    if (n2 <= &graph[i] && tfirst(n2)) continue;
                    output1 += n1->is_neighb[n2-&graph[0]];
                }
            }
        }
    }

    cout << "part1: " << output1 << endl;

    current_set.reserve(26*26);
    compute(0);
    
    cout << "part2: ";
    for (int k : largest_set) {
        print_node(k);
        if (k != largest_set.back()) cout << ",";
    }
    cout << endl;

    return 0;
}
