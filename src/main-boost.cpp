// Copyright (c) 2018 dphpc-ducks
#include "graph.hpp"
#include "graphviz.hpp"
#include "performance.hpp"

#include <boost/graph/prim_minimum_spanning_tree.hpp>

#include <iostream>
#include <fstream>
#include <vector>


/// A base iterator over the unique edges of a VectorGraph.
class UniqueEdgeIter {
protected:
	const VectorGraph::Edge *e, *m;

	void skip() {
		while (e != m && e->first > e->second)
			++e;
	}

public:
	UniqueEdgeIter(const VectorGraph::Edge *e, const VectorGraph::Edge *m): e(e), m(m) {
		skip();
	}

	bool operator== (const UniqueEdgeIter &other) const {
		return e == other.e;
	}

	bool operator!= (const UniqueEdgeIter &other) const {
		return e != other.e;
	}

	UniqueEdgeIter &operator++() {
		++e;
		skip();
		return *this;
	}
};

/// An iterator adaptor from VectorGraph edges to boost vertex pairs.
class EdgeIter : public UniqueEdgeIter {
public:
	EdgeIter(const VectorGraph::Edge *e, const VectorGraph::Edge *m): UniqueEdgeIter(e, m) {}

	std::pair<size_t, size_t> operator*() const {
		return std::make_pair(e->first, e->second);
	}
};

/// An iterator adaptor from VectorGraph edges to boost weight values.
class WeightIter : public UniqueEdgeIter {
public:
	WeightIter(const VectorGraph::Edge *e, const VectorGraph::Edge *m): UniqueEdgeIter(e, m) {}

	size_t operator*() const {
		return e->weight;
	}
};

int main(int argc, char **argv) {

	// Parse the command line arguments.
	if (argc != 2) {
		std::cerr << "usage: " << argv[0] << " GRAPH\n";
		return 1;
	}
	const char *GRAPH = argv[1];
	PerformanceTimer timer;

	// Load the graph from disk and convert to a boost-compatible graph.
	VectorGraph vg(GRAPH);
	Graph g(
		EdgeIter(vg.edges, vg.edges + vg.num_edges),
		EdgeIter(vg.edges + vg.num_edges, vg.edges + vg.num_edges),
		WeightIter(vg.edges, vg.edges + vg.num_edges),
		vg.num_vertices
	);

	// Compute the Minimum Spanning Tree using Prim's algorithm.
	std::vector<int> p(boost::num_vertices(g));
	timer.tick("graph_setup");
	prim_minimum_spanning_tree(g, &p[0]);
	timer.tick("minimum_spanning_tree");

	// Emit the Graphviz description of the graph and highlight the edges that
	// belong to the MST.
	if (boost::num_vertices(g) <= 100) {
		std::ofstream fout("boost_mst.gv");
		write_graphviz(fout, g, p);
	}
	timer.tick("write_result");

	return 0;
}
