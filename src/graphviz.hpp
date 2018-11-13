// Copyright (c) 2018 dphpc-ducks
#pragma once
#include "vector_graph.hpp"
#include <boost/graph/graphviz.hpp>

/// Convert an index to an alphabetic representation.
inline std::string cardinal_to_alphabetic(size_t i) {
	std::string s = "";
	do {
		s += 'A' + (i % 26);
		i /= 26;
	} while (i > 0);
	return s;
}

/// A writer that assigns letters to vertices to make reading graphs easier.
struct VertexWriter {
	template <class VertexOrEdge>
	void operator()(std::ostream& out, const VertexOrEdge& v) const {
		out << "[label=\"" << cardinal_to_alphabetic(v) << "\"]";
	}
};

/// A writer that labels edges with their edge weight.
struct EdgeWriter {
	const Graph &graph;

	EdgeWriter(const Graph &graph) : graph(graph) {}

	template <class VertexOrEdge>
	void operator()(std::ostream& out, const VertexOrEdge& v) const {
		out << "[label=\"" << boost::get(boost::edge_weight_t(), graph)[v] << "\"]";
	}
};

/// A writer that labels edges with their edge weight and highlights edges
/// belonging to the MST in red.
struct MstEdgeWriter {
	const Graph &graph;
	const std::vector<int> &mst;

	MstEdgeWriter(const Graph &graph, const std::vector<int> &mst) : graph(graph), mst(mst) {}

	template <class VertexOrEdge>
	void operator()(std::ostream& out, const VertexOrEdge& v) const {
		out << "[";
		out << "label=\"" << boost::get(boost::edge_weight_t(), graph)[v] << "\"";
		int src = boost::source(v, graph);
		int dst = boost::target(v, graph);
		if (mst[src] == dst || mst[dst] == src)
			out << ", color=red";
		out << "]";
	}
};

/// Write a Graphviz representation of a graph.
inline void write_graphviz(std::ostream& out, const Graph &g) {
	boost::write_graphviz(out, g, VertexWriter(), EdgeWriter(g));
}

/// Write a Graphviz representation of a graph, highlighting the edges that
/// belong to its Minimum Spanning Tree.
inline void write_graphviz(std::ostream& out, const Graph &g, const std::vector<int> &p) {
	boost::write_graphviz(out, g, VertexWriter(), MstEdgeWriter(g, p));
}

/// Write a Graphviz representation of a vector graph, highlighting the edges
/// that belong to its Minimum Spanning Tree.
inline void write_graphviz(std::ostream& out, const VectorGraph &g, const std::vector<size_t> &mst) {
	out << "graph G {\n";
	for (size_t i = 0; i < g.vertices; ++i) {
		out << "\t" << i << " [label=\"" << cardinal_to_alphabetic(i) << "\"];\n";
	}
	out << "\n";
	for (size_t i = 0; i < g.edges.size(); ++i) {
		const auto src = g.edges[i].first;
		const auto dst = g.edges[i].second;
		if (src > dst) continue;
		out << "\t" << src << " -- " << dst;
		out << " [label=\"" << g.edges[i].weight << "\"";
		if (!mst.empty() && (mst[src] == dst || mst[dst] == src)) {
			out << ", color=red";
		}
		out << "];\n";
	}
	out << "}\n";
}

/// Write a Graphviz representation of a vector graph.
inline void write_graphviz(std::ostream& out, const VectorGraph &g) {
	write_graphviz(out, g, std::vector<size_t>());
}
