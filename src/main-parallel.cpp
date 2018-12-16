// Copyright (c) 2018 dphpc-ducks
#include "graph.hpp"
#include "graphviz.hpp"
#include "vector_graph.hpp"
#include "partial_forest.hpp"
#include "performance.hpp"
#include "prim_msf.hpp"
#include "verification.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <thread>

/// Computes the minimum spanning forest.
int main(int argc, char **argv) {

	// Parse the command line arguments.
	if (argc != 3) {
		std::cerr << "usage: " << argv[0] << " NUM_VERTICES EDGE_PROB\n";
		return 1;
	}
	const uint64_t NUM_VERTICES = std::atoi(argv[1]);
	const double EDGE_PROB = std::atof(argv[2]);
	const uint64_t NUM_EDGES = NUM_VERTICES * (NUM_VERTICES-1) / 2 * EDGE_PROB;
	PerformanceTimer timer;

	// Load the graph from disk.
	VectorGraph g(NUM_VERTICES, NUM_EDGES);
	timer.tick("graph_setup");

	// Compute the Minimum Spanning Forest using Prim's algorithm.
	PartialForest msf(NUM_VERTICES);
	timer.tick("msf_setup");
	PrimMsf prim(g, msf);
	std::vector<std::thread> threads;
	const size_t NUM_THREADS = 4;
	for (size_t i = 0; i < NUM_THREADS; ++i) {
		threads.emplace_back(std::thread([&](){
			prim.run(NUM_VERTICES/NUM_THREADS, i);
		}));
	}
	std::cout << "waiting for sreads\n";
	for (auto &t : threads) {
		t.join();
	}
	timer.tick("minimum_spanning_tree");

	// Check that this is indeed a minimum spanning tree.
	// auto correct = verify_minimum_spanning_tree(pf, g);
	auto correct = false;
	timer.tick("verification");

	// Emit the Graphviz description of the graph and highlight the edges that
	// belong to the MST.
	if (g.num_vertices <= 100) {
		std::ofstream fout("parallel_msf.gv");
		write_graphviz(fout, g, msf);
	}
	timer.tick("write_result");

	// return !correct;
	return 0;
}
