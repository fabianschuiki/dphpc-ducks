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
		std::cerr << "usage: " << argv[0] << " NUM_VERTICES EDGE_PROB OUTPUT\n";
		return 1;
	}
	const uint64_t NUM_VERTICES = std::atoi(argv[1]);
	const double EDGE_PROB = std::atof(argv[2]);
	const uint64_t NUM_EDGES = NUM_VERTICES * (NUM_VERTICES-1) / 2 * EDGE_PROB;
	const char *OUTPUT = argv[3];
	PerformanceTimer timer;

	// Generate a random Erdös-Renyi graph.
	VectorGraph g(NUM_VERTICES, NUM_EDGES);
	timer.tick("setup");
	g.write(OUTPUT);
	timer.tick("write");

	return 0;
}
