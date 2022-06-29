#include "Vectorgraph.h"
#include <iomanip>

using namespace std;
graph::graph(std::string filename)
{
    //filename = "/../" + filename;
    std::ifstream file(filename.c_str());
    int numRow, numCol, numLines;
    int sum_adj = 0;
    double data;
    bool is_symetric = 0;
    std::string title;
    std::getline(file, title);
    std::ofstream fout("data2.txt");
    if (title == "")
    {
        fout << "something went wrong";
    }
    cout << title << "\n";
    if (title.find("symmetric") != std::string::npos)
    {
        is_symetric = 1;
    };
    cout << "Symmetric: " << is_symetric << "\n";
    std::cout << title << std::endl;
    while (file.peek() == '%')
    {
        file.ignore(2048, '\n');
    }
    file >> numRow >> numCol >> numLines;
    cout << numRow << " "  << " " << numCol << "  " << numLines << "\n";
    std::vector<std::vector<int> > tempAdj(numRow);
 
    for (int i = 0; i < numRow; i++) {
        rename.push_back(i);
    }
    for (int i = 0; i < numRow; i++) {
        renameRev.push_back(i);
    }
    nums.resize(numRow + 1);
    if (is_symetric)
    {
        secVer.resize(numLines);
    }
    secVer.resize(2 * numLines);
    for (int l = 0; l < numLines; l++)
    {
        int row, col;
        file >> row >> col >> data;
        //file >> row >> col;
        tempAdj[row - 1].push_back(col - 1);
        
        //if (is_symetric)
        //{
            tempAdj[col - 1].push_back(row - 1);
        //}
    }

    file.close();


    std::vector<std::pair<int, int>> vertNums;
    for (int i = 0; i < numRow; i++)
    {
        
        vertNums.push_back(make_pair(i,tempAdj[i].size()));
        
    }

    cout << "SORT\n";
    std::sort(vertNums.begin(), vertNums.end(),
        [](const pair<int, int>& a, const pair<int, int>& b) {return a.second > b.second; }); 


    vector<int> rename1(numRow, 0);
    vector<int> rename2(numRow, 0);
    cout << "SORTED PAIRS\n";

    for (int i = 0; i < vertNums.size(); i++)
    {

        rename1[i] = vertNums[i].first;
        rename2[vertNums[i].first] = i;
    }
    cout << "\n";


    cout << "RENAME\n";

    rename = rename1;
    renameRev = rename2;
    
    cout << "RENAME ADJ\n";
    for (int i = 0; i < numRow; i++)
    {
        for (int j = 0; j < tempAdj[i].size(); j++)
            tempAdj[i][j] = renameRev[tempAdj[i][j]];
    }


    nums[0] = 0;
    int counter = 0;
    for (int i = 0; i < rename.size(); i++)
    {
        for (int j = 0; j < tempAdj[rename[i]].size(); j++)
        {
            adj.push_back(tempAdj[rename[i]][j]);
            sum_adj++;
            secVer[counter] = rename[i];
            counter++;
        }
        nums[i + 1] = sum_adj;
    }
}
graph::graph(std::vector<int> a, std::vector<int> n, std::vector<int> sv) : adj(a), nums(n), secVer(sv) {}
graph::graph(std::vector<int> a, std::vector<int> n, std::vector<int> sv, std::vector<int> rn, std::vector<int> rnRev) : adj(a), nums(n), secVer(sv), rename(rn), renameRev(rnRev) {}
int find(std::vector<subtree>& t_tree, int i)
{
    int root = i;
    while (t_tree[root].parent != root)
    {
        root = t_tree[root].parent;
    }

    while (t_tree[i].parent != root)
    {
        int tmp = t_tree[i].parent;
        t_tree[i].parent = root;
        i = root;
    }

    return root;
}


int partition(std::vector<std::vector<int>>& vec, int l, int h, std::vector<int>& rename, std::vector<int>& renameRev)
{
    std::vector<int> x = vec[h];
    int i = (l - 1);

    for (int j = l; j <= h - 1; j++) {
        if (vec[j].size() >= x.size()) {
            i++;
            std::swap(vec[i], vec[j]);

            std::swap(rename[i], rename[j]);

            std::swap(renameRev[rename[i]], renameRev[rename[j]]);

        }
    }
    std::swap(vec[i + 1], vec[h]);

    std::swap(rename[i + 1], rename[h]);

    std::swap(renameRev[rename[i + 1]], renameRev[rename[h]]);

    return (i + 1);
}

void quickSortIterative(std::vector<vector<int>>& vec, std::vector<int>& rename, std::vector<int>& renameRev)
{
    int l = 0;
    int h = vec.size() - 1;
    std::vector<int> stack(h - l + 1, 0);
    int top = -1;
    stack[++top] = l;
    stack[++top] = h;

    while (top >= 0) {

        h = stack[top--];
        l = stack[top--];


        int p = partition(vec, l, h, rename, renameRev);


        if (p - 1 > l) {
            stack[++top] = l;
            stack[++top] = p - 1;
        }


        if (p + 1 < h) {
            stack[++top] = p + 1;
            stack[++top] = h;
        }
    }
}

int partition(std::vector<std::pair<int, int>>& vec, int l, int h, std::vector<int>& rename, std::vector<int>& renameRev)
{
    pair<int, int> x = vec[h];
    int i = (l - 1);

    for (int j = l; j <= h - 1; j++) {
        if (vec[j].second >= x.second) {
            i++;
            std::swap(vec[i], vec[j]);

            std::swap(rename[i], rename[j]);

            std::swap(renameRev[rename[i]], renameRev[rename[j]]);

        }
    }
    std::swap(vec[i + 1], vec[h]);

    std::swap(rename[i + 1], rename[h]);

    std::swap(renameRev[rename[i + 1]], renameRev[rename[h]]);

    return (i + 1);
}

void quickSortIterative(std::vector<std::pair<int, int>>& vec, std::vector<int>& rename, std::vector<int>& renameRev)
{
    int l = 0;
    int h = vec.size() - 1;
    std::vector<int> stack(h - l + 1, 0);
    int top = -1;
    stack[++top] = l;
    stack[++top] = h;

    while (top >= 0) {

        h = stack[top--];
        l = stack[top--];


        int p = partition(vec, l, h, rename, renameRev);
        if(p % 100000 == 0) 
            cout << p << "\n";


        if (p - 1 > l) {
            stack[++top] = l;
            stack[++top] = p - 1;
        }


        if (p + 1 < h) {
            stack[++top] = p + 1;
            stack[++top] = h;
        }
    }
}
