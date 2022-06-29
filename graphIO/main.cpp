#include "Vectorgraph.h"
#include <iomanip> 
#include <iostream>
using namespace std;
int main() {
    graph gr("../test_bfs.mtx");
    int y;
    cout << gr.adj.size() << endl;
    for (int i = 0; i < gr.adj.size(); i++)
    {
        cout << "(" << gr.adj[i] << "," << gr.secVer[i] << ")" << endl;
    }
    for (int i = 0; i < gr.nums.size(); i++)
    {
        cout << gr.nums[i] << " ";
    }
    std::cin >> y;
    return 0;
}