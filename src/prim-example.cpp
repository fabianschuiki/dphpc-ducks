// Copyright (c) 2018 dphpc-ducks
#include "graph.hpp"
#include "graphviz.hpp"

#include <boost/graph/prim_minimum_spanning_tree.hpp>

#include <iostream>
#include <vector>

int main(int argc, char **argv) {

	// Generate a fixed graph in memory.
	typedef std::pair<size_t, size_t> E;
	const size_t num_nodes = 5;

	std::vector<E> edges = {
		E(0, 2), E(1, 3), E(1, 4), E(2, 1), E(2, 3), E(3, 4), E(4, 0)
	};
	std::vector<size_t> weights = { 1, 1, 2, 7, 3, 1, 1 };
	Graph g(edges.begin(), edges.end(), &weights[0], num_nodes);
	{
		std::ofstream fout("example.gv");
		write_graphviz(fout, g);
	}

	// Compute the Minimum Spanning Tree using Prim's algorithm.
	std::vector<size_t> p(boost::num_vertices(g));
	prim_minimum_spanning_tree(g, &p[0]);
	{
		std::ofstream fout("example_mst.gv");
		write_graphviz(fout, g, p);
	}

	// Dump a list of parent nodes.
	for (std::size_t i = 0; i != p.size(); ++i)
		if (p[i] != i)
			std::cout << "parent[" << i << "] = " << p[i] << std::endl;
		else
			std::cout << "parent[" << i << "] = no parent" << std::endl;

	return 0;
}
