// Copyright (c) 2018 dphpc-ducks
#pragma once
#include <cstdint>
#include <cassert>
#include <iostream>

#include "lfsr_taps.hpp"

/// A maximum-length pseudo-random sequence generator.
///
/// This is a non-repeating random number generator based on Maximum Length
/// Sequences. It emits a sequence of random, non repeating, uniformly
/// distributed numbers. The length of the sequence and the range of the
/// generated number is configurable.
class MaxLenRandSeq {
	/// The range of the generated values as `[0,range)`.
	uint64_t range;
	/// The taps used for the LFSR.
	uint64_t taps;
	/// The current state of the generator.
	uint64_t state;

	/// Computes the next power of two of a number. Based on [1].
	///
	/// [1]: https://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
	static inline uint64_t next_power_of_two(uint64_t v) {
		v--;
		v |= v >> 1;
		v |= v >> 2;
		v |= v >> 4;
		v |= v >> 8;
		v |= v >> 16;
		v |= v >> 32;
		v++;
		return v;
	}

	/// A very inefficient way to compute the rounded-up base-2 logarithm of an
    /// integer.
	static inline uint64_t integer_clog2(uint64_t v) {
		uint64_t i;
		for (i = 0; i < 64 && ((uint64_t)1 << i) < v; ++i);
		return i;
	}

public:
	MaxLenRandSeq(uint64_t seed, uint64_t range) {
        ++range;
        this->range = range;
        uint64_t lg = integer_clog2(range);
        assert(lg > 0);
        uint64_t mask = ((uint64_t)1 << lg) - 1;
        this->taps = LFSR_TAPS[lg-1];
        this->state = (seed & mask) | 1;
	}

	/// Draw the next random number from the sequence.
	inline uint64_t operator()() {
		do {
            uint64_t lsb = state & 1;
            state >>= 1;
            state ^= (-lsb) & taps;
		} while (state >= range);
		return state-1;
	}
};
