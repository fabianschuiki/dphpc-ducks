# DPHPC Project

[![Build Status](https://travis-ci.org/fabianschuiki/dphpc-ducks.svg?branch=master)](https://travis-ci.org/fabianschuiki/dphpc-ducks)

This is our project work for "Design of Parallel and High-Performance Computing" (263-2800-00L) at ETHZ.

## Usage

Configure the project once at the beginning:

    mkdir build
    cd build
    cmake -DCMAKE_BUILD_TYPE=release ..

Then build the project:

    make

The project builds the following executables:

- `prim-example`: An example application of Prim's algorithm as implemented by the boost library [1].
- `mst-sequential`: A custom single-threaded sequential implementation used as the base building block for the parallelized version.
- `mst-boost`: A single-threaded sequential implementation based on Prim's algorithm in the boost library [1].
- `graphgen`: A graph generation utility that generates on-disk graphs.
- `bench-graph`: Benchmark for LFSR-based graph generation.

Once compiled you may use the following scripts to generate a sample Graphviz rendering of a random graph with its MST highlighted:

- `scripts/draw-example.sh`
- `scripts/draw-sequential.sh`
- `scripts/draw-boost.sh`

## Tests

The project includes some unit tests. Either run the `test-*` executables individually, or call

    make test

to run all unit tests in the project.

## Benchmark

To benchmark MST implementations, run the following scripts:

    scripts/benchmark-mst.sh boost
    scripts/benchmark-mst.sh sequential


[1]: https://www.boost.org/doc/libs/1_68_0/boost/graph/prim_minimum_spanning_tree.hpp
