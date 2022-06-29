#ifndef __TGRAPH_H__
#define __TGRAPH_H__
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <omp.h>
#include <fstream>
class graph
{
public:
    std::vector<int> adj;
    std::vector<int> nums;
    std::vector<int> secVer;
    std::vector<int> rename; 
    std::vector<int> renameRev;
    graph(std::string filename);
    graph(std::vector<int> a, std::vector<int> n, std::vector<int> sv);
    graph(std::vector<int> a, std::vector<int> n, std::vector<int> sv, std::vector<int> rn, std::vector<int> rnRev);
};
class subtree
{
public:
    int parent;
    int rank;
    subtree(int pr = 0, int r = 0)
    {
        parent = pr;
        rank = r;
    }
};
int partition(std::vector<std::vector<int>>& vec, int l, int h, std::vector<int>& rename, std::vector<int>& renameRev);
void quickSortIterative(std::vector<std::vector<int>>& vec, std::vector<int>& rename, std::vector<int>& renameRev);

int partition(std::vector<std::pair<int, int>>& vec, int l, int h, std::vector<int>& rename, std::vector<int>& renameRev);
void quickSortIterative(std::vector<std::pair<int, int>>& vec, std::vector<int>& rename, std::vector<int>& renameRev);

#endif
