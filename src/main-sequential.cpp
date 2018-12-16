// Copyright (c) 2018 dphpc-ducks
#include "graph.hpp"
#include "graphviz.hpp"
#include "vector_graph.hpp"
#include "partial_forest.hpp"
#include "performance.hpp"
#include "verification.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

/// Computes the minimum spanning forest.
void prim_minimum_spanning_forest(const VectorGraph &g, PartialForest &msf) {

	// Initialize an auxiliary structure that tracks the minimum weight leading
	// into/out of a vertex.
	std::vector<size_t> weights(g.num_vertices, std::numeric_limits<size_t>::max());

	// Keep a priority queue of vertices reachable from the current blob of
	// vertices, ordered by the corresponding edge's weight. First element of
	// the pair is the weight, second element is the vertex id.
	typedef std::pair<size_t, size_t> weight_vtx_id_t;
	std::priority_queue<
		weight_vtx_id_t,
		std::vector<weight_vtx_id_t>,
		std::greater<weight_vtx_id_t>>
	queue;

	// Keep working until all vertices have been incorporated into the MSF.
	while (!msf.complete()) {
		// Pick an initial free vertex at random and stage it as root to the MSF.
		size_t init = msf.random_free_vertex(0);
		msf.stage_vertex(init, init);

		// Add that vertex to the priority queue as initial step.
		weights[init] = std::numeric_limits<size_t>::min();
		queue.push(std::make_pair(weights[init], init));

		// Keep working possible next vertices off the priority queue.
		while (true) {
			// Drop vertices that are already in the MSF from the front of the queue.
			while (!queue.empty() && msf.contains_vertex(queue.top().second))
				queue.pop();
			// If the queue is empty, stop working on it.
			if (queue.empty())
				break;
			// Add the free vertex that is connected through the least-weight edge, v0, to the MSF.
			auto v0 = queue.top().second;
			queue.pop();
			msf.commit_vertex(v0);

			// Check each edge leading out of v0 for possible next vertices v1.
			auto adjacent = g.get_adjacent_vertices(v0);
			for (auto i = adjacent.first; i != adjacent.second; ++i) {
				const auto v1 = i->first == v0 ? i->second : i->first;
				const auto weight = i->weight;

				// If v1 is untouched and the weight of this edge is an
				// improvement over whatever we've seen before, update the
				// weight and add the key to the priority queue again.
				if (!msf.contains_vertex(v1) && weights[v1] > weight) {
					weights[v1] = weight;
					queue.push(std::make_pair(weight, v1));

					// Update the parent node in the MSF.
					msf.stage_vertex(v1, v0);
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
	timer.tick("graph_setup");

	// Compute the Minimum Spanning Forest using Prim's algorithm.
	PartialForest msf(NUM_VERTICES);
	timer.tick("msf_setup");
	prim_minimum_spanning_forest(g, msf);
	timer.tick("minimum_spanning_forest");

	// Check that this is indeed a minimum spanning tree.
	// auto correct = verify_minimum_spanning_tree(pf, g);
	auto correct = false;
	timer.tick("verification");

	// Emit the Graphviz description of the graph and highlight the edges that
	// belong to the MST.
	if (g.num_vertices <= 100) {
		std::ofstream fout("sequential_msf.gv");
		write_graphviz(fout, g, msf);
	}
	timer.tick("write_result");

	// return !correct;
	return 0;
}
