// Copyright (c) 2018 dphpc-ducks
#include "maxlenrandseq.hpp"
#include <iostream>
#include <set>

int main(int argc, char **argv) {
	if (argc != 2) {
		std::cerr << "usage: " << argv[0] << " RANGE\n";
		return 1;
	}

	std::set<uint64_t> seen;
	const uint64_t range = atoi(argv[1]);
	std::cout << "testing " << range << " samples\n";
	MaxLenRandSeq mls(0xdeadbeeffacefeedUL, range);

	uint64_t first_sample = 0;
	for (uint64_t i = 0; i < range; ++i) {
		uint64_t sample = mls();
		if (i == 0)
			first_sample = sample;
		if (!seen.insert(sample).second) {
			std::cerr << "sample " << sample << " produced twice (after " << i << " draws)\n";
			return 1;
		}
		if (sample >= range) {
			std::cerr << "sample " << sample << " beyond maximum " << range << " (after " << i << " draws)\n";
			return 1;
		}
	}

	uint64_t sample = mls();
	if (!seen.count(sample)) {
		std::cerr << "sequence should repeat after " << range << " samples, but produced unique " << sample << "\n";
		return 1;
	}
	if (sample != first_sample) {
		std::cerr << "does not repeat\n";
		return 1;
	}

	return 0;
}
