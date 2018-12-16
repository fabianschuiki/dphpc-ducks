// Copyright (c) 2018 dphpc-ducks
#include "prim_msf.hpp"
#include <queue>

void prim_minimum_spanning_forest(const VectorGraph &g, PartialForest &msf, size_t n_steps, size_t offset) {

	// Initialize an auxiliary structure that tracks the minimum weight leading
	// into/out of a vertex.
	std::vector<size_t> weights(g.num_vertices, std::numeric_limits<size_t>::max());

	// Keep a priority queue of vertices reachable from the current blob of
	// vertices, ordered by the corresponding edge's weight. First element of
	// the pair is the weight, second element is the vertex id.
	typedef std::pair<size_t, size_t> weight_vtx_id_t;
	std::priority_queue<
		weight_vtx_id_t,
		std::vector<weight_vtx_id_t>,
		std::greater<weight_vtx_id_t>>
	queue;

	// Keep working until all vertices have been incorporated into the MSF or specified number of
	// steps has been reached.
	size_t step = 0;
	while (!msf.complete() && step < n_steps) {
		// Pick an initial free vertex at random and stage it as root to the MSF.
		size_t init = msf.next_free_vertex(offset);
		if (init == PartialForest::nvtx)
			break;
		msf.stage_vertex(init, init);

		// Add that vertex to the priority queue as initial step.
		weights[init] = std::numeric_limits<size_t>::min();
		queue.push(std::make_pair(weights[init], init));

		// Keep working possible next vertices off the priority queue.
		while (step < n_steps) {
			// Drop vertices that are already in the MSF from the front of the queue.
			while (!queue.empty() && msf.contains_vertex(queue.top().second))
				queue.pop();
			// If the queue is empty, stop working on it.
			if (queue.empty())
				break;
			// Add the free vertex that is connected through the least-weight edge, v0, to the MSF.
			auto v0 = queue.top().second;
			queue.pop();
			msf.commit_vertex(v0);
			step++;

			// Check each edge leading out of v0 for possible next vertices v1.
			auto adjacent = g.get_adjacent_vertices(v0);
			for (auto i = adjacent.first; i != adjacent.second; ++i) {
				const auto v1 = i->first == v0 ? i->second : i->first;
				const auto weight = i->weight;

				// If v1 is untouched and the weight of this edge is an
				// improvement over whatever we've seen before, update the
				// weight and add the key to the priority queue again.
				if (!msf.contains_vertex(v1) && weights[v1] > weight) {
					weights[v1] = weight;
					queue.push(std::make_pair(weight, v1));

					// Update the parent node in the MSF.
					msf.stage_vertex(v1, v0);
				}
			}
		}
	}
}
