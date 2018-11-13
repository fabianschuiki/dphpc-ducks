// Copyright (c) 2018 dphpc-ducks
#include "graph.hpp"
#include "graphviz.hpp"
#include "vector_graph.hpp"

#include <iostream>
#include <fstream>
#include <vector>

/// Computes the minimum spanning tree.
///
/// The array `mst` must have the same number of elements as the graph has
/// vertices. The MST is represented by storing at each location in the array
/// the index of the connected vertex corresponding to location's vertex.
void prim_minimum_spanning_tree(const VectorGraph &g, size_t *mst) {
	// Pick the first node. We use node 0 here for convenience, but this could
	// be any random node in the graph.
	for (auto e : g.edges) {
		std::cout << "(" << e.first << ", " << e.second << ")\n";
	}

}

int main(int argc, char **argv) {

	// Parse the command line arguments.
	if (argc != 3) {
		std::cerr << "usage: " << argv[0] << " NUM_VERTICES NUM_EDGES\n";
		return 1;
	}
	const int NUM_VERTICES = std::atoi(argv[1]);
	const int NUM_EDGES = std::atoi(argv[2]);

	// Generate a random Erdös-Renyi graph.
	VectorGraph g(NUM_VERTICES, NUM_EDGES);

	// Compute the Minimum Spanning Tree using Prim's algorithm.
	std::vector<size_t> p(g.vertices);
	prim_minimum_spanning_tree(g, &p[0]);

	// Emit the Graphviz description of the graph and highlight the edges that
	// belong to the MST.
	{
		std::ofstream fout("sequential_mst.gv");
		write_graphviz(fout, g, p);
	}

	return 0;
}
