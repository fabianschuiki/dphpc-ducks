# DPHPC Project

This is our project work for "Design of Parallel and High-Performance Computing" (263-2800-00L) at ETHZ.

## Usage

Configure the project once at the beginning:

    mkdir build
    cd build
    cmake -DCMAKE_BUILD_TYPE=release ..

Then build the project:

    make

The project builds the following executables:

- `mst-sequential`: A single-threaded sequential implementation based on Prim's algorithm in the boost library [1].


[1]: https://www.boost.org/doc/libs/1_68_0/boost/graph/prim_minimum_spanning_tree.hpp
