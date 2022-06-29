#include "bfs.h"
#include <iomanip> 
#include <iostream>
#include <algorithm>
#include <omp.h>

using namespace std;

int main() {
    double read = omp_get_wtime();
    read = omp_get_wtime();
    vector<int> ad = readBinar("LJ_adj_s");
    cout << "reading time: " << omp_get_wtime() - read << "\n";
    vector<int> b = readBinar("LJ_num_s");
    cout << "reading time: " << omp_get_wtime() - read << "\n";
    vector<int> c = readBinar("LJ_num_s");
    vector<int> rn = readBinar("LJ_rename_s");
    cout << "reading time: " << omp_get_wtime() - read << "\n";
    vector<int> rn2 = readBinar("LJ_renameRev_s");
    cout << "reading time: " << omp_get_wtime() - read << "\n";
    graph gr(ad, b, c, rn, rn2);
    int num;
    int m = 1;

    num = 4;
    cout << "threads: " << num << "\n";
    omp_set_num_threads(num);

    bfsOpenMP(gr, 1);
    bfsOpenMPBU(gr, 1);
    bfsOpenMPHB(gr, 1);
    return 0;
}
