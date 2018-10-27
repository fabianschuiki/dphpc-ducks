cmake_minimum_required(VERSION 2.8)
project(dphpc-ducks)

add_definitions(-Wall -std=c++11)

add_executable(mst-sequential src/main-sequential.cpp)
add_executable(prim-example src/prim-example.cpp)
