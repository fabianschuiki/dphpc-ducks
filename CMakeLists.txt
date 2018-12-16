cmake_minimum_required(VERSION 2.8)
project(dphpc-ducks)

find_package(Boost REQUIRED)

add_definitions(-Wall -std=c++11)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(src)

add_executable(mst-sequential src/prim_msf.cpp src/main-sequential.cpp)
add_executable(mst-boost src/main-boost.cpp)

add_executable(prim-example src/prim-example.cpp)
add_executable(graphgen src/main-graphgen.cpp)
add_executable(bench-graph src/bench-graph.cpp)

# Unit Tests
enable_testing()
add_executable(test_uniform_pairs test/uniform_pairs.cpp)
add_executable(test_maxlenrandseq test/maxlenrandseq.cpp)
add_executable(test_partial_forest test/partial_forest.cpp)
add_executable(test_prim_merge_forests src/prim_msf.cpp test/prim_merge_forests.cpp)
add_test(NAME uniform_pairs COMMAND test_uniform_pairs 1000)
add_test(NAME maxlenrandseq COMMAND test_maxlenrandseq 1000000)
add_test(NAME partial_forest COMMAND test_partial_forest)
add_test(NAME prim_merge_forests COMMAND test_prim_merge_forests)
