// Copyright (c) 2018 dphpc-ducks
#include "uniform_pairs.hpp"
#include <iostream>
#include <set>

int main(int argc, char **argv) {
	if (argc != 2) {
		std::cerr << "usage: " << argv[0] << " NUM_VERTICES\n";
		return 1;
	}

	const size_t max = atoi(argv[1]);
	const size_t num_pairs = max * (max - 1) / 2;
	std::cout << "testing " << max << " nodes, " << num_pairs << " pairs\n";

	size_t num_duplicates = 0;
	std::set<std::pair<int, int>> seen;
	for (auto it = UniformPairs<int>(max); it != UniformPairs<int>(); ++it) {
		// std::cout << "(" << it->first << ", " << it->second << ")\n";
		if (!seen.insert(*it).second) {
			++num_duplicates;
			std::cerr << "ERROR: (" << it->first << ", " << it->second << ") produced twice\n";
		}
	}
	if (seen.size() != num_pairs) {
		std::cerr << "ERROR: only produced " << seen.size() << " out of " << num_pairs << " pairs\n";
		return 1;
	}
	return num_duplicates > 0;
}
