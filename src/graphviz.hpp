// Copyright (c) 2018 dphpc-ducks
#pragma once
#include <boost/graph/graphviz.hpp>

/// A writer that assigns letters to vertices to make reading graphs easier.
struct VertexWriter {
	template <class VertexOrEdge>
	void operator()(std::ostream& out, const VertexOrEdge& v) const {
		int i = v;
		std::string s = "";
		do {
			s += 'A' + (i % 26);
			i /= 26;
		} while (i > 0);
		out << "[label=\"" << s << "\"]";
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
