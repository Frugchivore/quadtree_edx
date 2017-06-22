cmake_minimum_required(VERSION 3.7)
project(quadtree)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp QuadTree.cpp QuadTree.h interval.h)
add_executable(quadtree ${SOURCE_FILES})