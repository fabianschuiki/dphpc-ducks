// Copyright (c) 2018 dphpc-ducks
#pragma once
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/erdos_renyi_generator.hpp>
#include <boost/random/linear_congruential.hpp>
#include <random>

/// A graph for which an MST may be calculated.
///
/// We assume our graphs to adhere to this structure, i.e. to be undirected
/// weighted graphs. Taken from [1].
///
/// [1]: https://www.boost.org/doc/libs/1_68_0/libs/graph/example/prim-example.cpp
typedef boost::adjacency_list<
	boost::vecS,
	boost::vecS,
	boost::undirectedS,
	boost::property<boost::vertex_distance_t, int>,
	boost::property<boost::edge_weight_t, int>
> Graph;

/// An iterator producing random weights.
class RandomWeightIterator {
	std::minstd_rand rng;
	int max;
	int value;
public:
	RandomWeightIterator(std::minstd_rand rng, int max) : rng(rng), max(max) {
		++(*this);
	}

	int operator *() const {
		return value;
	}

	RandomWeightIterator& operator ++() {
		value = rng() % max;
		return *this;
	}
};

/// Create an Erdös-Renyi graph with V vertices and E edges and random weights.
inline Graph generate_erdos_renyi_graph(size_t V, size_t E) {
	typedef boost::erdos_renyi_iterator<std::minstd_rand, Graph> edge_iter;
	std::minstd_rand rng(42);
	return Graph(edge_iter(rng, V, E), edge_iter(), RandomWeightIterator(rng, E), V);
}
