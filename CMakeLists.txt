cmake_minimum_required(VERSION 2.8)
project(dphpc-ducks)

add_definitions(-Wall -std=c++11)
include_directories(src)

add_executable(mst-sequential src/main-sequential.cpp)
add_executable(mst-boost src/main-boost.cpp)

add_executable(prim-example src/prim-example.cpp)
add_executable(graphgen src/main-graphgen.cpp)

# Unit Tests
enable_testing()
add_executable(test_uniform_pairs test/uniform_pairs.cpp)
add_executable(test_maxlenrandseq test/maxlenrandseq.cpp)
add_test(NAME uniform_pairs COMMAND test_uniform_pairs 1000)
add_test(NAME maxlenrandseq COMMAND test_maxlenrandseq 1000000)
