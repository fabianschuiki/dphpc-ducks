// Copyright (c) 2018 dphpc-ducks
#pragma once
#include <chrono>
#include <iostream>

/// A helper class to measure the time between distinct program points.
class PerformanceTimer {
	typedef std::chrono::time_point<std::chrono::high_resolution_clock> time_t;
	time_t start;
	std::ostream &os;

	static time_t now() {
		return std::chrono::high_resolution_clock::now();
	}

public:
	PerformanceTimer(std::ostream &os): start(now()), os(std::cout) {}
	PerformanceTimer(): PerformanceTimer(std::cout) {}

	double tick(const char *msg = nullptr) {
		time_t end = now();
		std::chrono::duration<double> diff = end-start;
		if (msg)
			os << msg << ": " << diff.count() << " s\n";
		start = now();
		return diff.count();
	}
};
