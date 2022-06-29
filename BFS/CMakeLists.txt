cmake_minimum_required(VERSION 3.12...3.18)

project(${MST_LIBRARY} LANGUAGES CXX)
# delete comments to set C++ standart to C++17
# set(CMAKE_CXX_STANDART 17) 
# set(CMAKE_CXX_STANDART_REQUIRED ON)
# set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCE_EXE main.cpp)
 

add_library(${BFS_LIBRARY} STATIC
	source/bfs.cpp
	include/bfs.h
	)

target_include_directories(${BFS_LIBRARY} PUBLIC include)

add_executable(BFS_main ${SOURCE_EXE})

if(OpenMP_CXX_FOUND)
	target_link_libraries( ${BFS_LIBRARY} PUBLIC OpenMP::OpenMP_CXX)
endif()

target_link_libraries( ${BFS_LIBRARY} PUBLIC ${IO_LIBRARY})

target_link_libraries(BFS_main PUBLIC ${BFS_LIBRARY})