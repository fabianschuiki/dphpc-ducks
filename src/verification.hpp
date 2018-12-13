// Copyright (c) 2018 dphpc-ducks
#pragma once
#include <iostream>
#include <boost/dynamic_bitset.hpp>

typedef std::set<std::set<size_t>> SubgraphSet;

/// Determine the set of unconnected subgraphs.
SubgraphSet
gather_subgraphs(const Graph &graph) {
	SubgraphSet subgraphs;
	boost::dynamic_bitset<size_t> untouched;
	untouched.resize(boost::num_vertices(graph), true);
	while (untouched.any()) {
		std::set<size_t> vertices;
		std::set<size_t> todo;
		size_t init = untouched.find_first();
		todo.insert(init);
		while (!todo.empty()) {
			size_t vertex = *todo.begin();
			untouched.reset(vertex);
			todo.erase(vertex);
			vertices.insert(vertex);
			auto adjacent = boost::adjacent_vertices(vertex, graph);
			while (adjacent.first != adjacent.second) {
				size_t v = *adjacent.first;
				++adjacent.first;
				if (!untouched[v])
					continue;
				todo.insert(v);
			}
		}
		subgraphs.emplace(std::move(vertices));
	}
	return subgraphs;
}

/// Determine the set of unconnected subgraphs.
SubgraphSet
gather_subgraphs(const VectorGraph &graph) {
	SubgraphSet subgraphs;
	boost::dynamic_bitset<size_t> untouched;
	untouched.resize(graph.num_vertices, true);
	while (untouched.any()) {
		std::set<size_t> vertices;
		std::set<size_t> todo;
		size_t init = untouched.find_first();
		todo.insert(init);
		while (!todo.empty()) {
			size_t vertex = *todo.begin();
			untouched.reset(vertex);
			todo.erase(vertex);
			vertices.insert(vertex);
			auto adjacent = graph.get_adjacent_vertices(vertex);
			while (adjacent.first != adjacent.second) {
				size_t v = adjacent.first->second;
				++adjacent.first;
				if (!untouched[v])
					continue;
				todo.insert(v);
			}
		}
		subgraphs.emplace(std::move(vertices));
	}
	return subgraphs;
}

/// Determine the subgraphs spanned by a tree.
SubgraphSet
gather_spanned_subgraphs(
	const size_t *tree,
	size_t num_vertices
) {
	// Invert the tree representation to facilitate a flood fill.
	std::vector<std::vector<size_t>> children;
	std::vector<size_t> roots;
	children.resize(num_vertices);
	for (size_t i = 0; i < num_vertices; ++i) {
		if (tree[i] == i) {
			roots.push_back(i);
		} else {
			children[tree[i]].push_back(i);
		}
	}

	// Compute the subgraphs.
	SubgraphSet subgraphs;
	for (size_t root : roots) {
		std::set<size_t> vertices;
		std::set<size_t> todo;
		todo.insert(root);
		while (!todo.empty()) {
			size_t vertex = *todo.begin();
			todo.erase(vertex);
			vertices.insert(vertex);
			for (size_t v : children[vertex])
				todo.insert(v);
		}
		subgraphs.emplace(std::move(vertices));
	}
	return subgraphs;
}

/// Determine the number of vertices in a graph.
inline size_t
get_number_of_vertices(const Graph &graph) {
	return boost::num_vertices(graph);
}

/// Determine the number of vertices in a graph.
inline size_t
get_number_of_vertices(const VectorGraph &graph) {
	return graph.num_vertices;
}

/// Check whether the given `tree` is a Minimum Spanning Tree of `graph`.
template <typename G> bool
verify_minimum_spanning_tree(
	const size_t *tree,
	const G &graph
){
	// Compute the separate subgraphs of the graph, then do the same for the
	// spanning tree.
	auto subgraphs = gather_subgraphs(graph);
	auto subtrees = gather_spanned_subgraphs(tree, get_number_of_vertices(graph));

	// Ensure that the set of vertices touched by the forest of MSTs exactly
	// match the connected components in the original graph.
	SubgraphSet diff_plus, diff_minus;
	for (auto &sg : subgraphs)
		if (!subtrees.count(sg))
			diff_minus.insert(sg);
	for (auto &sg : subtrees)
		if (!subgraphs.count(sg))
			diff_plus.insert(sg);

	// If the two sets of subgraphs don't match, emit some diagnostics as to
	// what is missing, and where.
	auto correct = diff_plus.empty() && diff_minus.empty();
	std::cout << "graph has " << subgraphs.size() << " separate subgraphs\n";
	if (!correct) {
		std::cout << "mst spans " << subtrees.size() << " separate subgraphs\n";
		std::cout << diff_minus.size() << " subgraphs not in MST, " << diff_plus.size() << " subgraphs not in graph\n";
		for (auto &sg : diff_minus) {
			std::cout << " missing in MST (" << sg.size() << " nodes): { ";
			for (auto v : sg)
				std::cout << v << " ";
			std::cout << "}\n";
		}
		for (auto &sg : diff_plus) {
			std::cout << " missing in graph (" << sg.size() << " nodes): { ";
			for (auto v : sg)
				std::cout << v << " ";
			std::cout << "}\n";
		}
	}

	return correct;
}
