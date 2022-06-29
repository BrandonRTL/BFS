#ifndef __BFS_H__
#define __BFS_H__
#include "Vectorgraph.h"
#include <time.h>
#include <list>
#include <climits>
#include <iostream>
#include <vector>
#include <string>

void bfsOpenMP(graph& gr, int ind);
void bfsOpenMPBU(graph& gr, int ind);
void bfsOpenMPHB(graph& gr, int ind);

std::vector<int> readBinar(std::string name = "");
int writeBinar(std::vector<int>& vec, std::string name = "");

std::vector<int> readBinar_cl(std::string name = "");
int writeBinar_cl(std::vector<int>& vec, std::string name = "");
#endif
