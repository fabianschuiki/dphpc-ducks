// Copyright (c) 2018 dphpc-ducks
#pragma once
#include <chrono>


/// A helper class to measure the time between distinct program points.
class PerformanceTimer {
	typedef std::chrono::time_point<std::chrono::high_resolution_clock> time_t;
	time_t start;

	static time_t now() {
		return std::chrono::high_resolution_clock::now();
	}

public:
	PerformanceTimer(): start(now()) {}

	void tick(const char *msg) {
		time_t end = now();
		std::chrono::duration<double> diff = end-start;
		std::cout << msg << ": " << diff.count() << " s\n";
		start = now();
	}
};
