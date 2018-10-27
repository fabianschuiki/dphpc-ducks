// Copyright (c) 2018 dphpc-ducks
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>

#include <iostream>
#include <vector>

int main(int argc, char **argv) {

	typedef std::pair <int, int> E;
	typedef boost::adjacency_list<
		boost::vecS,
		boost::vecS,
		boost::undirectedS,
		boost::property<boost::vertex_distance_t, int>,
		boost::property<boost::edge_weight_t, int>
	> Graph;

	const int num_nodes = 5;
	std::vector<E> edges = {
		E(0, 2), E(1, 3), E(1, 4), E(2, 1), E(2, 3), E(3, 4), E(4, 0)
	};
	std::vector<int> weights = { 1, 1, 2, 7, 3, 1, 1 };

	Graph g(edges.begin(), edges.end(), &weights[0], num_nodes);

	std::vector<boost::graph_traits<Graph>::vertex_descriptor> p(boost::num_vertices(g));

	prim_minimum_spanning_tree(g, &p[0]);

	for (std::size_t i = 0; i != p.size(); ++i)
		if (p[i] != i)
			std::cout << "parent[" << i << "] = " << p[i] << std::endl;
		else
			std::cout << "parent[" << i << "] = no parent" << std::endl;

	return 0;
}
