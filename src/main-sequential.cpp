// Copyright (c) 2018 dphpc-ducks
#include "graph.hpp"
#include "graphviz.hpp"
#include "vector_graph.hpp"
#include "performance.hpp"

#include <iostream>
#include <fstream>
#include <vector>

/// Establishes a weight-dominant order between edges.
struct WeightCompare {
	bool operator()(
		const VectorGraph::Edge &a,
		const VectorGraph::Edge &b
	) const {
		if (a.weight < b.weight) return true;
		if (a.weight > b.weight) return false;
		if (a.first < b.first) return true;
		if (a.first > b.first) return false;
		return a.second < b.second;
	}
};

/// Comparator for searching edges related to a vertex.
struct FirstVertexCompare {
	bool operator()(const VectorGraph::Edge &a, size_t b) const {
		return a.first < b;
	}

	bool operator()(size_t a, const VectorGraph::Edge &b) const {
		return a < b.first;
	}
};

/// Computes the minimum spanning tree.
///
/// The array `mst` must have the same number of elements as the graph has
/// vertices. The MST is represented by storing at each location in the array
/// the index of the connected vertex corresponding to location's vertex.
void prim_minimum_spanning_tree(const VectorGraph &g, size_t *mst) {
	// std::cout << "ALL EDGES:\n";
	// for (auto e : g.edges) {
	// 	std::cout << e << "\n";
	// }

	// Pick the first node. We use node 0 here for convenience, but this could
	// be any random node in the graph.
	std::set<VectorGraph::Edge, WeightCompare> possible_edges;
	std::set<size_t> vertices;
	vertices.insert(0);
	for (size_t i = 0; i < g.num_edges && g.edges[i].first == 0; ++i) {
		possible_edges.insert(g.edges[i]);
	}
	// std::cout << "INIT:\n";
	// for (auto e : possible_edges) {
	// 	std::cout << e << "\n";
	// }

	// Iteratively pick the lowest-weight edge out of the cluster.
	for (size_t i = 0; i < 1000 && !possible_edges.empty(); ++i) {
		// Pick the lowest cost edge.
		VectorGraph::Edge edge = *possible_edges.begin();
		size_t new_vertex = edge.second;
		// std::cout << "ITERATION " << i << " ADD " << cardinal_to_alphabetic(edge.first) << " -- " << cardinal_to_alphabetic(edge.second) << " (" << possible_edges.size() << " possible)\n";
		vertices.insert(new_vertex);

		// Add the edge to the MST.
		mst[edge.second] = edge.first;

		// Remove all possible edges related to the added vertex. This gets rid
		// of any potential intra-cluster edges.
		size_t first_edge = std::distance(g.edges, std::lower_bound(g.edges, g.edges + g.num_edges, new_vertex, FirstVertexCompare()));
		size_t last_edge = std::distance(g.edges, std::upper_bound(g.edges, g.edges + g.num_edges, new_vertex, FirstVertexCompare()));
		if (first_edge == last_edge)
			break;
		// std::cout << "edges = " << first_edge << " .. " << last_edge << "\n";
		for (size_t i = first_edge; i < last_edge; ++i) {
			VectorGraph::Edge e = g.edges[i];
			// std::cout << e << "\n";
			if (!vertices.count(e.second))
				possible_edges.insert(e);
			std::swap(e.first, e.second);
			possible_edges.erase(e);
		}

		// Dump the edges left over.
		// std::cout << "possible next edges:\n";
		// for (auto e : possible_edges) {
		// 	std::cout << e << "\n";
		// }
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

	// Emit the Graphviz description of the graph and highlight the edges that
	// belong to the MST.
	if (g.num_vertices <= 100) {
		std::ofstream fout("sequential_mst.gv");
		write_graphviz(fout, g, p);
	}
	timer.tick("write_result");

	return 0;
}
