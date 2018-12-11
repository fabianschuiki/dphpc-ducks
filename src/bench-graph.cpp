// Copyright (c) 2018 dphpc-ducks
#include "graph.hpp"
#include "vector_graph.hpp"
#include "performance.hpp"

#include <iostream>
#include <vector>


static double bench_edge_generation(size_t N) {
	PerformanceTimer timer;
	UniformPairsSeq ups(N*20);
	volatile uint64_t chksum = 0;
	for (size_t i = 0; i < N; ++i) {
		volatile UniformPairsSeq::pair_t p = ups();
		chksum ^= p.first;
		chksum ^= p.second;
	}
	return timer.tick();
}

static double bench_vector(size_t N) {
	PerformanceTimer timer;
	std::vector<UniformPairsSeq::pair_t> pairs;
	pairs.reserve(N*2);
	UniformPairsSeq ups(N*20);
	for (size_t i = 0; i < N; ++i) {
		UniformPairsSeq::pair_t p = ups();
		pairs.push_back(p);
		std::swap(p.first, p.second);
		pairs.push_back(p);
	}
	volatile uint64_t chksum = 0;
	for (auto p : pairs) {
		chksum ^= p.first;
		chksum ^= p.second;
	}
	return timer.tick();
}

static double bench_vector_sorted(size_t N) {
	PerformanceTimer timer;
	// PerformanceTimer msgtimer;
	std::vector<UniformPairsSeq::pair_t> pairs;
	pairs.reserve(N*2);
	// msgtimer.tick("reserve");
	UniformPairsSeq ups(N*20);
	for (size_t i = 0; i < N; ++i) {
		UniformPairsSeq::pair_t p = ups();
		pairs.push_back(p);
		std::swap(p.first, p.second);
		pairs.push_back(p);
	}
	// msgtimer.tick("gen");
	std::sort(pairs.begin(), pairs.end());
	// msgtimer.tick("sort");
	volatile uint64_t chksum = 0;
	for (auto p : pairs) {
		chksum ^= p.first;
		chksum ^= p.second;
	}
	// msgtimer.tick("chksum");
	return timer.tick();
}

static double bench_set(size_t N) {
	PerformanceTimer timer;
	// PerformanceTimer msgtimer;
	std::set<UniformPairsSeq::pair_t> pairs;
	UniformPairsSeq ups(N*20);
	for (size_t i = 0; i < N; ++i) {
		UniformPairsSeq::pair_t p = ups();
		pairs.insert(p);
		std::swap(p.first, p.second);
		pairs.insert(p);
	}
	// msgtimer.tick("gen");
	volatile uint64_t chksum = 0;
	for (auto p : pairs) {
		chksum ^= p.first;
		chksum ^= p.second;
	}
	// msgtimer.tick("chksum");
	return timer.tick();

}

static double bench_vg(size_t N) {
	PerformanceTimer timer;
	size_t Np = N*20;
	size_t V = (sqrt(8.0*Np+1) + 1) / 2;
	std::cerr << "VectorGraph(" << V << ", " << N << ")\n";
	VectorGraph vg(V, N);
	return timer.tick();
}

int main(int argc, char **argv) {
	const static size_t sizes[] = {
		10,
		100,
		1000,
		10000,
		100000,
		1000000,
		// 10000000,
		// 100000000,
	};
	const static size_t num_sizes = sizeof(sizes)/sizeof(*sizes);

	for (size_t i = 0; i < num_sizes; ++i) {
		const size_t N = sizes[i];
		double d_edgegen = bench_edge_generation(N);
		double d_vector = bench_vector(N);
		double d_sorted = bench_vector_sorted(N);
		double d_set = bench_set(N);
		double d_vg = bench_vg(N);
		std::cout << N;
		std::cout << ", " << d_edgegen << ", " << d_edgegen/N;
		std::cout << ", " << d_vector << ", " << d_vector/N;
		std::cout << ", " << d_sorted << ", " << d_sorted/N;
		std::cout << ", " << d_set << ", " << d_set/N;
		std::cout << ", " << d_vg << ", " << d_vg/N;
		std::cout << "\n";
	}

	return 0;
}
