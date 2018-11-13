// Copyright (c) 2018 dphpc-ducks
#pragma once
#include "graph.hpp"

/// An efficient graph implementation based on index vectors.
///
/// Represents a graph as a vertex count and two lists: one containing pairs of
/// vertex indices to represent the edges, and one containing the weight of each
/// edge. For the list of edges the following invariants hold:
///
/// - the first vertex index is smaller than the second vertex index
/// - the list is sorted by first and then by second index
class VectorGraph {
public:
	struct Edge {
		size_t first;
		size_t second;
		size_t weight;
	};

	size_t vertices;
	std::vector<Edge> edges;

	/// Create a new graph based on edge and weight iterators.
	///
	/// Creates a new graph with `vertices` vertices. The iterator pair
	/// `edge_begin` and `edge_end` is used to generate edges as pairs of vertex
	/// indices. The `weight_iter` is used to allocate weights to the each of
	/// the edges. The edges can be emitted in any order and will be sorted for
	/// maximum lookup efficiency.
	template <typename EdgeIter, typename WeightIter>
	VectorGraph(
		EdgeIter edge_begin,
		EdgeIter edge_end,
		WeightIter weight_iter,
		size_t vertices
	): vertices(vertices) {
		for (; edge_begin != edge_end; ++edge_begin, ++weight_iter) {
			edges.push_back(Edge{
				.first = (*edge_begin).first,
				.second = (*edge_begin).second,
				.weight = *weight_iter
			});
		}
		std::sort(edges.begin(), edges.end(), [&](const Edge &a, const Edge &b){
			if (a.first < b.first)
				return true;
			if (a.first > b.first)
				return false;
			return a.second < b.second;
		});
	}

	/// Create an Erdös-Renyi graph with V vertices and E edges and random
	/// weights.
	VectorGraph(size_t V, size_t E): VectorGraph(
		UniformPairs<size_t>(V, E),
		UniformPairs<size_t>(),
		RandomWeightIterator<size_t>(E),
		V
	) {}
};
