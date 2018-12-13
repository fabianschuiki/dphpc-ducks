// Copyright (c) 2018 dphpc-ducks
#include "graph.hpp"
#include "graphviz.hpp"
#include "vector_graph.hpp"
#include "performance.hpp"
#include "verification.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

/// Computes the minimum spanning tree.
///
/// The array `mst` must have the same number of elements as the graph has
/// vertices. The MST is represented by storing at each location in the array
/// the index of the connected vertex corresponding to location's vertex.
void prim_minimum_spanning_tree(const VectorGraph &g, size_t *mst) {
	// Initialize a bit set that tracks which vertices are not part of the MST
	// yet.
	boost::dynamic_bitset<size_t> untouched;
	untouched.resize(g.num_vertices, true);

	// Initialize an auxiliary structure that tracks the minimum weight leading
	// into/out of a vertex.
	std::vector<size_t> weights(g.num_vertices, std::numeric_limits<size_t>::max());

	// Keep a priority queue of vertices reachable from the current blob of
	// vertices, ordered by the corresponding edge's weight. First element of
	// the pair is the weight, second element is the vertex id.
	std::priority_queue<std::pair<size_t, size_t>> queue;

	// Keep working until all vertices have been incorporated into an MST.
	while (untouched.any()) {
		// Pick the next vertex that is not part of an MST.
		size_t init = untouched.find_first();
		untouched.reset(init);

		// Add that vertex to the priority queue as initial step.
		weights[init] = std::numeric_limits<size_t>::min();
		queue.push(std::make_pair(weights[init], init));

		// Keep working possible next vertices off the priority queue.
		while (!queue.empty()) {
			// Pop the next item off the front of the queue.
			auto v0 = queue.top().second;
			queue.pop();

			// Mark the vertex as discovered.
			untouched.reset(v0);

			// Check each edge leading out of v0 for possible next vertices v1.
			auto adjacent = g.get_adjacent_vertices(v0);
			for (auto i = adjacent.first; i != adjacent.second; ++i) {
				auto v1 = i->second;
				auto weight = i->weight;

				// If v1 is untouched or the weight of this edge is an
				// improvement over whatever we've seen before, update the
				// weight and add the key to the priority queue again.
				if (untouched[v1] && weights[v1] > weight) {
					weights[v1] = weight;
					queue.push(std::make_pair(weight, v1));

					// Update the parent node in the MST.
					mst[v1] = v0;
				}
			}
		}
	}
}

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

	// Compute the Minimum Spanning Tree using Prim's algorithm.
	std::vector<size_t> p(g.num_vertices);
	std::iota(p.begin(), p.end(), 0);
	timer.tick("graph_setup");
	prim_minimum_spanning_tree(g, &p[0]);
	timer.tick("minimum_spanning_tree");

	// Check that this is indeed a minimum spanning tree.
	auto correct = verify_minimum_spanning_tree(&p[0], g);
	timer.tick("verification");

	// Emit the Graphviz description of the graph and highlight the edges that
	// belong to the MST.
	if (g.num_vertices <= 100) {
		std::ofstream fout("sequential_mst.gv");
		write_graphviz(fout, g, p);
	}
	timer.tick("write_result");

	return !correct;
}
