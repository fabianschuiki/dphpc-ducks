// Copyright (c) 2018 dphpc-ducks
#include "graph.hpp"
#include "vector_graph.hpp"
#include "performance.hpp"

#include <iostream>
#include <fstream>
#include <vector>


int main(int argc, char **argv) {

	// Parse the command line arguments.
	if (argc != 4) {
		std::cerr << "usage: " << argv[0] << " NUM_VERTICES NUM_EDGES OUTPUT\n";
		return 1;
	}
	const int NUM_VERTICES = std::atoi(argv[1]);
	const int NUM_EDGES = std::atoi(argv[2]);
	const char *OUTPUT = argv[3];
	PerformanceTimer timer;

	// Generate a random Erdös-Renyi graph.
	VectorGraph g(NUM_VERTICES, NUM_EDGES);
	timer.tick("setup");
	g.write(OUTPUT);
	timer.tick("write");

	return 0;
}
