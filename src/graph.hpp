// Copyright (c) 2018 dphpc-ducks
#pragma once
#include "uniform_pairs.hpp"

#include <boost/graph/adjacency_list.hpp>
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
template <typename T>
class RandomWeightIterator {
	std::mt19937 rng;
	std::uniform_int_distribution<T> dist;
	T value;
public:
	RandomWeightIterator(T max): dist(0, max-1) {
		++(*this);
	}

	T operator *() const {
		return value;
	}

	RandomWeightIterator& operator ++() {
		value = dist(rng);
		return *this;
	}
};

/// Create an Erdös-Renyi graph with V vertices and E edges and random weights.
inline Graph generate_erdos_renyi_graph(size_t V, size_t E) {
	return Graph(
		UniformPairs<size_t>(V, E),
		UniformPairs<size_t>(),
		RandomWeightIterator<size_t>(E),
		V
	);
}
