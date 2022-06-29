cmake_minimum_required(VERSION 3.12...3.18)

set(PROJECT_NAME graphAlgorithms)

project(${PROJECT_NAME})

set(IO_LIBRARY graphIO)
set(BFS_LIBRARY BFS)


find_package(OpenMP)

#subprojects
add_subdirectory(graphIO)
add_subdirectory(BFS)
