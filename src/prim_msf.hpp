// Copyright (c) 2018 dphpc-ducks
#pragma once
#include "partial_forest.hpp"
#include "vector_graph.hpp"

/// An implementation of Prim's algorithm with various early-abort features.
class PrimMsf {
	const VectorGraph &graph;
	PartialForest &msf;

	// An auxiliary structure that tracks the minimum weight leading into/out of
	// a vertex.
	std::vector<size_t> weights;

public:
	PrimMsf(const VectorGraph &graph, PartialForest &msf):
		graph(graph),
		msf(msf),
		weights(graph.num_vertices, std::numeric_limits<size_t>::max()) {}

	void run(size_t n_steps, size_t offset);
};
