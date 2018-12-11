// Copyright (c) 2018 dphpc-ducks
#pragma once
#include "graph.hpp"
#include "performance.hpp"

#include <fstream>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

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

private:
	/// The backing vector for edges in case the graph owns its memory.
	std::vector<Edge> owned_edges;
	/// The memory-mapped file data.
	void *mmap_data = nullptr;
	/// The size of the memory-mapped file data.
	size_t mmap_size = 0;

public:
	size_t num_vertices;
	size_t num_edges;
	const Edge *edges = nullptr;

	/// Create a new graph based on edge and weight iterators.
	///
	/// Creates a new graph with `vertices` vertices. The `edge_iter` is used to
	/// generate edges as pairs of vertex indices. The `weight_iter` is used to
	/// allocate weights to the each of the edges. The edges can be emitted in
	/// any order and will be sorted for maximum lookup efficiency.
	template <typename EdgeIter, typename WeightIter>
	VectorGraph(
		EdgeIter edge_iter,
		WeightIter weight_iter,
		size_t num_vertices,
		size_t num_edges
	): num_vertices(num_vertices), num_edges(num_edges) {
		PerformanceTimer timer;
		owned_edges.reserve(num_edges);
		timer.tick("VectorGraph.reserve");
		for (size_t i = 0; i < num_edges; ++i, ++edge_iter, ++weight_iter) {
			owned_edges.push_back(Edge{
				.first = (*edge_iter).first,
				.second = (*edge_iter).second,
				.weight = *weight_iter
			});
			owned_edges.push_back(Edge{
				.first = (*edge_iter).second,
				.second = (*edge_iter).first,
				.weight = *weight_iter
			});
		}
		timer.tick("VectorGraph.edges");
		std::sort(owned_edges.begin(), owned_edges.end(), [&](const Edge &a, const Edge &b){
			if (a.first < b.first)
				return true;
			if (a.first > b.first)
				return false;
			return a.second < b.second;
		});
		timer.tick("VectorGraph.sorting");
		edges = &owned_edges[0];
	}

	/// Create an Erdös-Renyi graph with V vertices and E edges and random
	/// weights.
	VectorGraph(size_t V, size_t E): VectorGraph(
		UniformPairs<size_t>(V, E),
		RandomWeightIterator<size_t>(E),
		V, E
	) {}

	/// Create a graph based on a file on disk.
	///
	/// Uses mmap to efficiently map the data into memmory.
	VectorGraph(const char *filename) {
		// Memory map the file into memory.
		int fd = open(filename, O_RDONLY);
		if (fd == -1) {
			perror("open");
			exit(1);
		}

		struct stat fs;
		if (fstat(fd, &fs) == -1) {
			perror("fstat");
			close(fd);
			exit(1);
		}

		if (!S_ISREG(fs.st_mode)) {
			fprintf(stderr, "not a regular file\n");
			close(fd);
			exit(1);
		}

		mmap_size = fs.st_size;
		mmap_data = mmap(0, mmap_size, PROT_READ, MAP_SHARED, fd, 0);
		if (mmap_data == MAP_FAILED) {
			perror("mmap");
			close(fd);
			exit(1);
		}

		if (close(fd) == -1) {
			perror("close");
			exit(1);
		}

		// Parse the data.
		size_t *ptr = (size_t *)mmap_data;
		num_vertices = ptr[0];
		num_edges = ptr[1];
		edges = (const Edge *)&ptr[2];
	}

	~VectorGraph() {
		// If data has been memory mapped, unmap it.
		if (mmap_data) {
			if (munmap(mmap_data, mmap_size) == -1) {
				perror("munmap");
				exit(1);
			}
			mmap_data = nullptr;
		}
	}

	/// Write the graph to an output stream.
	///
	/// The file layout is as follows:
	/// - number of vertices (size_t)
	/// - number of edges (size_t)
	/// - list of edges as vertex-vertex-weight triplets (3x size_t each)
	void write(std::ostream &os) const {
		os.write((const char *)&num_vertices, sizeof(size_t));
		os.write((const char *)&num_edges, sizeof(size_t));
		os.write((const char *)edges, num_edges * sizeof(Edge));
	}

	/// Write the graph to a file on disk.
	void write(const char *filename) const {
		std::ofstream f(filename);
		write(f);
	}
};

inline std::ostream &operator<< (std::ostream &os, const VectorGraph::Edge &e) {
	os << "(" << e.first << " -- " << e.second << "; " << e.weight << ")";
	return os;
}
