#include "bfs.h"
#include <iomanip> 
#include <iostream>
#include <algorithm>

using namespace std;

void bfsOpenMP(graph& gr, int ind)
{
    int vSize = gr.nums.size() - 1;
    int vertLeft = vSize;
    int edgesLeft = gr.adj.size();
    vector<int> levels(vSize, -1);
    
    vector<double> threadTimes(omp_get_max_threads(), 0);
    std::ofstream fout("times.txt");

    int print_size = levels.size();
    if (print_size > 100)
    {
        print_size = 100;
    }

    std::vector<int> adj(gr.adj);
    std::vector<int> nums(gr.nums);
    std::cout << "Size: " << vSize << " , " << adj.size() << "\n";

    int k, u;
    ind = gr.renameRev[ind]; 
    levels[ind] = 0;
    k = 1;
    int temp = 0;

    vector<int> frontierMask(vSize, 0);
    vector<int> next(vSize, 0);
    for (int i = 0; i < vSize; i++)
    {
        frontierMask[i] = 0;
    }
    frontierMask[ind] = 1;

    int check_seq = 1;
    cout << "BFS OpenMP SYCL-LIKE" << std::endl;
    cout << vertLeft << " " << edgesLeft << std::endl;
    double duration = 0;
    double durationInit = 0;
    double durationUpd = 0;
    double durationCheck = 0;
    double durationCopy = 0;

    double durationUpdIter = 0;

    double start = 0;
    double end = 0;
    double startInit = 0;
    double endInit = 0;
    double startUpd = 0;
    double endUpd = 0;
    double startCheck = 0;
    double endCheck = 0;
    double startCopy = 0;
    double endCopy = 0;

    start = omp_get_wtime();
    while (check_seq != 0)
    {
        check_seq = 0;
        startInit = omp_get_wtime();
#pragma omp parallel for
        for (int i = 0; i < vSize; i++)
        {
            next[i] = 0;
        }
        endInit = omp_get_wtime();
        durationInit += (endInit - startInit);
      

        startUpd = omp_get_wtime();
#pragma omp parallel for  private(u)  schedule(dynamic, 1000)
        for (int i = 0; i < vSize; i++)
        {
            u = frontierMask[i];
            if (u != 0)
            {
                for (int j = gr.nums[i]; j < gr.nums[i + 1]; j++)
                {
                    int temp = gr.adj[j];   
                    if (levels[temp] == -1)
                    {
                        next[temp] = 1;
                        levels[temp] = k;
                    }
                }
            }
        }

        endUpd = omp_get_wtime();
        durationUpdIter = (endUpd - startUpd);
        for (int i = 0; i < threadTimes.size(); i++)
        {
            threadTimes[i] = 0;
        }
        durationUpd += durationUpdIter;

        startCheck = omp_get_wtime();
        for (int i = 0; i < vSize; i++)
        {
            if (next[i] != 0)
            {
                check_seq = check_seq + 1;
                break;
            }
        }
        endCheck = omp_get_wtime();
        durationCheck = (endCheck - startCheck);

        startCopy = omp_get_wtime();

#pragma omp parallel for
        for (int i = 0; i < vSize; i++)
        {
            frontierMask[i] = next[i];
        }

        endCopy = omp_get_wtime();
        durationCopy += (endCopy - startCopy);

        k = k + 1;
    }
    end = omp_get_wtime();

    duration = end - start;
    //cout << "Omp time: " << duration  << endl;
    //cout << "Omp init time: " << durationInit << endl;
    cout << "Omp update time: " << durationUpd << endl;
    //cout << "Omp check time: " << durationCheck << endl;
    //cout << "Omp copy time: " << durationCopy << endl;
  
    //for (int i = 0; i < print_size; i++) // ÏÅÐÅÈÌÅÍÎÂÊÀ
    //{
    //    cout << levels[gr.renameRev[i]] << " ";
    //}
    //cout << std::endl;

    for (int i = 0; i < print_size; i++)
    {
        cout << levels[i] << " ";
    }
    cout << std::endl;
}


void bfsOpenMPBU(graph& gr, int ind)
{
    int vSize = gr.nums.size() - 1;
    vector<int> levels(vSize, -1);

    int print_size = levels.size();
    if (print_size > 100)
    {
        print_size = 100;
    }

    ind = gr.renameRev[ind]; 

    std::vector<int> adj(gr.adj);
    std::vector<int> nums(gr.nums);
    std::cout << "Size: " << vSize << " , " << adj.size() << "\n";

    int k, u;
    levels[ind] = 0;
    k = 0;
    int temp = 0;

    vector<int> frontierMask(vSize, 0);
    vector<int> next(vSize, 0);
    for (int i = 0; i < vSize; i++)
    {
        frontierMask[i] = 0;
    }
    frontierMask[ind] = 1;

    int check_seq = 1;
    cout << "BFS OpenMP SYCL-LIKE BU" << std::endl;

    double duration = 0;
    double durationInit = 0;
    double durationUpd = 0;
    double durationCheck = 0;
    double durationCopy = 0;

    double durationUpdIter = 0;

    double start = 0;
    double end = 0;
    double startInit = 0;
    double endInit = 0;
    double startUpd = 0;
    double endUpd = 0;
    double startCheck = 0;
    double endCheck = 0;
    double startCopy = 0;
    double endCopy = 0;

    start = omp_get_wtime();
    int isChanged = 1;
    while (isChanged)
    {
        isChanged = 0;
        check_seq = 0;

        startInit = omp_get_wtime();

#pragma omp parallel for
        for (int i = 0; i < vSize; i++)
        {
            next[i] = 0;
        }

        endInit = omp_get_wtime();
        durationInit += (endInit - startInit);
        startUpd = omp_get_wtime();

#pragma omp parallel for  private(temp) schedule(dynamic, 1000)
        for (int i = 0; i < vSize; i++)
        {
            if (levels[i] == -1)
            {
                for (int j = gr.nums[i]; j < gr.nums[i + 1]; j++)
                {
                    int temp = gr.adj[j];
                    if (levels[temp] == k)
                    {
                        next[i] = 1;
                        levels[i] = k + 1;
                        isChanged = 1;
                        break;
                    }
                }
            }
            
        }

        endUpd = omp_get_wtime();
        durationUpdIter = (endUpd - startUpd);
        durationUpd += durationUpdIter;

        startCheck = omp_get_wtime();

        for (int i = 0; i < vSize; i++)
        {
            if (next[i] != 0)
            {
                check_seq = check_seq + 1;
                break;
            }
        }

        endCheck = omp_get_wtime();
        durationCheck = (endCheck - startCheck);

        startCopy = omp_get_wtime();

#pragma omp parallel for
        for (int i = 0; i < vSize; i++)
        {
            frontierMask[i] = next[i];
        }

        endCopy = omp_get_wtime();
        durationCopy += (endCopy - startCopy);

        k = k + 1;
    }

    end = omp_get_wtime();

    
    duration = end - start;
    //cout << "Omp time: " << duration << endl;
    //cout << "Omp init time: " << durationInit << endl;
    cout << "Omp update time: " << durationUpd << endl;
  /*  cout << "Omp check time: " << durationCheck << endl;
    cout << "Omp copy time: " << durationCopy << endl;*/
  /*  for (int i = 0; i < print_size; i++)
    {
        cout << levels[i] << " ";
    }*/
    cout << std::endl;
}

void bfsOpenMPHB(graph& gr, int ind)
{
    int vSize = gr.nums.size() - 1;
    int vertLeft = vSize;
    int edgesLeft = gr.adj.size();
    int curVert = 0;
    int curEdge = 0;
    int curHub = 0;
    int hubVert = 0;

    ind = gr.renameRev[ind]; 
     
    vector<int> levels(vSize, -1);
    vector<int> vertsFront;
    vector<int> edgesFront;


    int print_size = levels.size();
    if (print_size > 100)
    {
        print_size = 100;
    }

    std::vector<int> adj(gr.adj);
    std::vector<int> nums(gr.nums);
    std::cout << "Size: " << vSize << " , "  << adj.size() <<  "\n";

    int k, u;
    levels[ind] = 0;
    k = 1;
    int temp = 0;

    vector<int> frontierMask(vSize, 0);
    vector<int> next(vSize, 0);
    for (int i = 0; i < vSize; i++)
    {
        frontierMask[i] = 0;
    }
    frontierMask[ind] = 1;

    int check_seq = 1;
    cout << "BFS OpenMP SYCL-LIKE hybrid" << std::endl;
    cout << vertLeft << " " << edgesLeft << std::endl;

    double duration = 0;
    double durationInit = 0;
    double durationUpd = 0;
    double durationRdc = 0;
    double durationRdcEgde = 0;
    double durationRdcVert = 0;
    double durationRdcUnfizEdge = 0;
    double durationCheck = 0;
    double durationCopy = 0;

    double durationUpdIter = 0;

    double start = 0;
    double end = 0;
    double startInit = 0;
    double endInit = 0;
    double startUpd = 0;
    double endUpd = 0;
    double startRdc = 0;
    double endRdc = 0;
    double startRdcVert = 0;
    double endRdcVert = 0;
    double startRdcUnfizEdge = 0;
    double endRdcUnfizEdge = 0;
    double startRdcEdge = 0;
    double endRdcEdge = 0;
    double startCheck = 0;
    double endCheck = 0;
    double startCopy = 0;
    double endCopy = 0;

    bool isTD = 1;

    double SwitchParam = 0.1;
  
//  
//    std::vector<std::pair<int, int>> adj_s;
//    for (int i = 0; i < vSize; i++)
//    {
//        for (int j = gr.nums[i]; j < gr.nums[i + 1]; j++)
//        {
//            adj_s.push_back(make_pair(gr.adj[j], gr.nums[gr.adj[j] + 1] - gr.nums[gr.adj[j]]));
//        }
//    }
//
//
//    //for (int i = 0; i < vSize; i++)
//    //{
//    //    std::sort(std::begin(adj_s) + gr.nums[i], std::begin(adj_s) + gr.nums[i + 1],
//    //        [](const pair<int, int>& a, const pair<int, int>& b) {return a.second > b.second; });
//    //}
//
// 
//#pragma omp parallel for
//    for (int i = 0; i < adj.size(); i++)
//    {
//        gr.adj[i] = adj_s[i].first;
 //   }

    start = omp_get_wtime();
    while (check_seq != 0)
    {
        curEdge = 0;
        curVert = 0;
        curHub = 0;
        check_seq = 0;

        startRdc = omp_get_wtime();
        startRdcVert = omp_get_wtime();
#pragma omp parallel for reduction(+:curVert)
        for (int i = 0; i < vSize; i++)
        {
            if (frontierMask[i] == 1)
            {
                curVert += 1;
            }
        }
        vertLeft -= curVert;
       
        vertsFront.push_back(curVert);
        endRdcVert = omp_get_wtime();
        durationRdcVert += (startRdcVert - endRdcVert);


        startRdcEdge = omp_get_wtime();
#pragma omp parallel for reduction(+:curEdge)
        for (int i = 0; i < vSize; i++)
        {
            if (frontierMask[i] == 1)
            {
                curEdge += nums[i + 1] - nums[i];
            }
        }
        
        edgesFront.push_back(curEdge);
        endRdcEdge = omp_get_wtime();
        durationRdcEgde += (startRdcEdge - endRdcEdge);



        curEdge = 0;
        startRdcUnfizEdge = omp_get_wtime();

        if (curVert)
        {
            check_seq = 1;
        }
        endRdc = omp_get_wtime();
        durationRdcUnfizEdge += (startRdcUnfizEdge - endRdc);
        durationRdc += (endRdc - startRdc);

   
        
        if (k > 1) {
            if (isTD) {
                if (edgesFront[k - 1] > edgesLeft / 14 && edgesFront[k - 1] > edgesFront[k - 2]) {
                    isTD = 0;
                }
            }
            else {
                if (vertsFront[k - 1] < vSize / 24 && vertsFront[k - 1] < vertsFront[k - 2]) {
                    isTD = 1;
                }
            }
   
        }
        

        startInit = omp_get_wtime();
#pragma omp parallel for
        for (int i = 0; i < vSize; i++)
        {
            next[i] = 0;
        }
        endInit = omp_get_wtime();
        durationInit += (endInit - startInit);

        startUpd = omp_get_wtime();
        if (isTD)
        {
#pragma omp parallel for schedule(dynamic, 1000)
            for (int i = 0; i < vSize; i++)
            {
                
                int u = frontierMask[i];
                if (u != 0)
                {
                    for (int j = gr.nums[i]; j < gr.nums[i + 1]; j++)
                    {
                        int temp = gr.adj[j];
                        if (levels[temp] == -1)
                        {
                            next[temp] = 1;
                            levels[temp] = k;
                        }
                    }
                }
            }
        }
        else 
        {
#pragma omp parallel for  private(temp)  schedule(guided)
            for (int i = 0; i < vSize; i++)
            {
                if (levels[i] == -1)
                {
                    for (int j = gr.nums[i]; j < gr.nums[i + 1]; j++)
                    {
                        int temp = gr.adj[j];
                        if (levels[temp] == k - 1)
                        {
                            next[i] = 1;
                            levels[i] = k;
                            break;
                        }
                    }
                }
            }
        }

        endUpd = omp_get_wtime();
        durationUpdIter = (endUpd - startUpd);
        durationUpd += durationUpdIter;

        startCopy = omp_get_wtime();

#pragma omp parallel for
        for (int i = 0; i < vSize; i++)
        {
            frontierMask[i] = next[i];
        }

        endCopy = omp_get_wtime();
        durationCopy += (endCopy - startCopy);

        k = k + 1;

    }
    end = omp_get_wtime();

    duration = end - start;
    cout << "Omp time: " << duration << endl;
    cout << "Omp init time: " << durationInit << endl;
    cout << "Omp update time: " << durationUpd << endl;
    cout << "Omp reduction time: " << durationRdc << endl;
    cout << "Omp edge reduction time: " << durationRdcEgde << endl;
    cout << "Omp vert reduction time: " << durationRdcVert << endl;
    cout << "Omp unvis reduction time: " << durationRdcUnfizEdge << endl;
    cout << "Omp check time: " << durationCheck << endl;
    cout << "Omp copy time: " << durationCopy << endl;
    for (int i = 0; i < print_size; i++)
    {
        cout << levels[i] << " ";
    }
}

int writeBinar(vector<int>& vec, string name)
{
    string fileName = name + ".bin";
    ofstream o(fileName, ios_base::binary);

    unsigned Xsize = 0;
 
    Xsize = vec.size();
    o.write(reinterpret_cast<char *>(&Xsize), sizeof(unsigned));
    o.write(reinterpret_cast<char *>(&vec[0]), vec.size() * sizeof(int));

    o.close();
    return 0;
}

vector<int> readBinar(string name)
{
    string fileName = name + ".bin";
    ifstream in(fileName, ios_base::binary);
    unsigned vsize;
    in.read(reinterpret_cast<char *>(&vsize), sizeof(unsigned));
    vector<int> theints(vsize);
    in.read(reinterpret_cast<char *>(&theints[0]), vsize * sizeof(int));

    in.close();
    return theints;
}

int writeBinar_cl(std::vector<int>& vec, std::string name) {
    string fileName = name + ".bin";
    std::ofstream fout(fileName.c_str());
    fout << vec.size() << "\n";
    for (int i = 0; i < vec.size(); i++) {
        fout << vec[i] << "\n";
    }
    return 0;
}

std::vector<int> readBinar_cl(std::string name) {
    vector<int> res;
    string fileName = name + ".bin";
    std::ifstream file(fileName.c_str());
    int size;
    file >> size; 
    for (int i = 0; i < size; i++) {
        int temp;
        file >> temp;
        res.push_back(temp);
    }
    return res;
}
