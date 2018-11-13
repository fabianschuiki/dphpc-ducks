// Copyright (c) 2018 dphpc-ducks
#include "graph.hpp"
#include "graphviz.hpp"
#include "performance.hpp"

#include <boost/graph/prim_minimum_spanning_tree.hpp>

#include <iostream>
#include <fstream>
#include <vector>


int main(int argc, char **argv) {

	// Parse the command line arguments.
	if (argc != 3) {
		std::cerr << "usage: " << argv[0] << " NUM_VERTICES NUM_EDGES\n";
		return 1;
	}
	const int NUM_VERTICES = std::atoi(argv[1]);
	const int NUM_EDGES = std::atoi(argv[2]);
	PerformanceTimer timer;

	// Generate a random Erdös-Renyi graph.
	Graph g = generate_erdos_renyi_graph(NUM_VERTICES, NUM_EDGES);

	// Compute the Minimum Spanning Tree using Prim's algorithm.
	std::vector<int> p(boost::num_vertices(g));
	timer.tick("graph_setup");
	prim_minimum_spanning_tree(g, &p[0]);
	timer.tick("minimum_spanning_tree");

	// Emit the Graphviz description of the graph and highlight the edges that
	// belong to the MST.
	{
		std::ofstream fout("boost_mst.gv");
		write_graphviz(fout, g, p);
	}
	timer.tick("write_result");

	return 0;
}
