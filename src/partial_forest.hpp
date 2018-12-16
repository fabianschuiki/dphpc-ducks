// Copyright (c) 2018 dphpc-ducks
#pragma once

#include <boost/dynamic_bitset.hpp>
#include <iostream>
#include <random>
#include <sys/types.h>
#include <vector>

class PartialForest {
public:
	typedef size_t vtx_id_t;

private:
	std::vector<vtx_id_t> parent_ids;
	boost::dynamic_bitset<> free_vertices;
	size_t num_vertices;
	std::minstd_rand* rand_gen;

	void init_rand() {
		std::random_device rd;
		rand_gen = new std::minstd_rand(rd());
	}

public:
	/// Create an empty partial forest.
	PartialForest(const size_t num_vertices) : num_vertices(num_vertices) {
		parent_ids.resize(num_vertices);
		free_vertices.resize(num_vertices, true);
		init_rand();
	}

	/// Create a partial forest by copying another one.
	PartialForest(const PartialForest& other) :
			free_vertices(other.free_vertices), num_vertices(other.num_vertices) {
		parent_ids.resize(num_vertices);
		std::copy(other.parent_ids.begin(), other.parent_ids.end(), parent_ids.begin());
		init_rand();
	}

	/// Return the number of vertices that this forest can hold at most.
	size_t capacity() const {
		return num_vertices;
	}

	/// Determine if this forest is complete.
	bool complete() const {
		return free_vertices.none();
	}

	/// Determine if this forest contains a vertex.
	inline bool contains_vertex(const vtx_id_t& vertex_id) const {
		return !free_vertices[vertex_id];
	}

	/// Determine the next vertex that is not yet in this forest.
	vtx_id_t next_free_vertex(vtx_id_t min_id) const {
		if (min_id == 0)
			return free_vertices.find_first();
		else
			return free_vertices.find_next(min_id - 1);
	}

	/// Determine a random vertex that is not yet in this forest.
	vtx_id_t random_free_vertex(vtx_id_t min_id) const {
		assert(min_id < capacity());
		std::uniform_int_distribution<vtx_id_t> dist(min_id, capacity() - 1);
		const size_t num_vertices = capacity() - min_id;
		for (size_t trial = 0; trial < num_vertices; ++trial) {
			const auto id = dist(*rand_gen);
			if (free_vertices[id])
				return id;
		}
		if (min_id == 0)
			return free_vertices.find_first();
		else
			return next_free_vertex(min_id);
	}

	/// Merge another partial forest into this one.
	PartialForest& merge(const PartialForest& other) {
		assert(num_vertices == other.num_vertices);

		// First pass: For all vertices that are in both forests, connect the trees following the
		// ancestry defined in *this* forest.
		for (unsigned i = 0; i < num_vertices; ++i) {
			// TODO: Can this traversal be reduced by keeping track of visited vertices?
			if (contains_vertex(i) && other.contains_vertex(i) &&
					(parent_id(i) != other.parent_id(i))) {
				auto cur = i;
				while (true) {
					const auto next = other.parent_id(cur);
					if (!contains_vertex(next)) {
						add_vertex(next, cur);
					}
					else
						break;
					if (other.vertex_is_root(next))
						break;
					cur = next;
				}
			}
		}

		// Second pass: All vertices that are in the other forest but still not in this one can be
		// added to this forest with the original ancestry, because they are disjoint with the trees
		// currently in this forest.
		for (unsigned i = 0; i < num_vertices; ++i) {
			// TODO: Can this traversal be reduced by keeping track of roots?
			if (!contains_vertex(i) && other.contains_vertex(i)) {
				add_vertex(i, other.parent_id(i));
			}
		}

		return *this;
	}

	/// Stage a vertex for addition to this partial forest.  This gives the vertex a possible
	/// parent, but does not yet commit to that parent.
	void stage_vertex(const vtx_id_t vertex_id, const vtx_id_t parent_id) {
		assert(vertex_id < capacity());
		assert(free_vertices[vertex_id]);
		parent_ids[vertex_id] = parent_id;
	}

	/// Commit vertex to this partial forest.  This marks the vertex as non-free, after which its
	/// parent may no longer be changed.
	void commit_vertex(const vtx_id_t vertex_id) {
		assert(vertex_id < capacity());
		assert(free_vertices[vertex_id]);
		free_vertices.reset(vertex_id);
	}

	/// Add a vertex to this partial forest.  This stages and commits a vertex to a parent.
	PartialForest& add_vertex(const vtx_id_t vertex_id, const vtx_id_t parent_id) {
		stage_vertex(vertex_id, parent_id);
		commit_vertex(vertex_id);

		return *this;
	}

	/// Determine the parent ID of a given vertex ID in this forest. Not defined if that ID is not
	/// in this forest.
	inline vtx_id_t parent_id(const vtx_id_t& vertex_id) const {
		assert(contains_vertex(vertex_id));
		return parent_ids[vertex_id];
	}

	/// Determine if a given vertex ID is the root of a tree in this forest. Not defined if that ID
	/// is not in this forest.
	inline bool vertex_is_root(const vtx_id_t& vertex_id) const {
		assert(contains_vertex(vertex_id));
		return (parent_id(vertex_id) == vertex_id);
	}

	/// Determine if this partial forest is equal to an other.
	bool operator==(const PartialForest& other) const {
		if (num_vertices != other.num_vertices)
			return false;

		if (free_vertices != other.free_vertices)
			return false;

		return (parent_ids == other.parent_ids);
	}

	/// Determine if this partial forest is not equal to an other.
	bool operator!=(const PartialForest& other) const {
		return !(*this == other);
	}

	friend std::ostream& operator<<(std::ostream& os, const PartialForest& pf);
};

inline std::ostream &operator<<(std::ostream &os, const PartialForest& pf) {
	for (unsigned i = 0; i < pf.num_vertices; ++i) {
		os << i << "\t";
		if (pf.contains_vertex(i)) {
			os << pf.parent_id(i);
			if (pf.vertex_is_root(i))
				os << "\t(root)";
		}
		else
			os << "-";
		os << std::endl;
	}
	return os;
}
