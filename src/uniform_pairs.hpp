// Copyright (c) 2018 dphpc-ducks
#pragma once
#include <memory>
#include <random>
#include <cassert>

/// A distribution of gaps in an integer range. This class may be used to avoid
/// drawing the same number twice. If empty it represents the entire configured
/// range. Once gaps are added, it provides a mapping from a count of remaining
/// integers to an actual integer. This allows e.g. a randomly drawn integer to
/// between 0 and the number of integers left in the range to be used to
/// directly draw an integer that has not yet been drawn before.
template <typename T>
class GapDistribution {
	/// A node in the binary tree representing the gaps ("skips").
	class Node {
		friend class GapDistribution;

		/// How many holes there are inside this node.
		T skips = 0;
		/// The lowest integer represented by this node.
		T lower = 0;
		/// The highest integer represented by this node, exclusive.
		T upper;
		/// The child nodes of the tree, if any. Either none or both are
		/// present.
		std::unique_ptr<Node> left, right;

		Node(T skips, T lower, T upper):
			skips(skips),
			lower(lower),
			upper(upper) {}

		Node(std::unique_ptr<Node> &&left, std::unique_ptr<Node> &&right):
			skips(left->skips + right->skips),
			lower(left->lower),
			upper(right->upper),
			left(std::move(left)),
			right(std::move(right)) {}

		Node(const Node &other):
			skips(other.skips),
			lower(other.lower),
			upper(other.upper),
			left(new Node(*other.left)),
			right(new Node(*other.right)) {}

		T project(const T &index) const {
			if (left && right) {
				T skipped = index + left->skips;
				if (skipped >= right->lower)
					return right->project(skipped);
				else
					return left->project(index);
			}
			return index;
		}

		bool remove(const T &value) {
			assert(value >= lower && value < upper);

			// If this is a leaf node which covers more than one integer, split
			// it up and create the appropriate child nodes which we will recur
			// into.
			if (!left && !right) {
				if (upper > lower + 1) {
					T middle = (lower + upper) / 2;
					left.reset(new Node(0, lower, middle));
					right.reset(new Node(0, middle, upper));
				} else {
					// We arrived at a node which only contains one integer. If the
					// skip has already been registered, return false to indicate
					// that nothing was removed.
					if (skips == 0) {
						++skips;
						return true;
					} else {
						return false;
					}
				}
			}

			// If control reaches this point, the child nodes have been created
			// and we simply have to recur.
			bool removed;
			if (value < right->lower)
				removed = left->remove(value);
			else
				removed = right->remove(value);
			if (removed)
				++skips;
			return removed;
		}
	};

	Node root;

public:
	GapDistribution(T lower, T upper): root(0, lower, upper) {}

	/// Get the number of gaps in the distribution.
	size_t count() const {
		return root.skips;
	}

	/// Map an index to an integer in the distribution, skipping over gaps.
	T operator()(const T &index) const {
		return root.project(index);
	}

	/// Remove an integer from the distribution, effectively creating a gap.
	void remove(const T &value) {
		root.remove(value);
	}
};

/// An iterator that yields a uniform distribution of unique integer pairs
/// within a certain range. The pairs are unique in the sense that the same
/// combination of integers is not produced twice, order not being significant.
template <typename T>
class UniformPairs {
public:
	typedef std::pair<T, T> pair_t;

private:
	struct Inner {
		T limit;
		T max_pairs;
		GapDistribution<T> gaps;
		std::mt19937 rng;

		Inner(T limit, T max_pairs):
			limit(limit),
			max_pairs(max_pairs),
			gaps(0, limit * (limit - 1)),
			rng() {}
	};

	std::shared_ptr<Inner> data;
	pair_t current;

	void update() {
		// Nothing to update if this is the end iterator.
		if (!data)
			return;

		// Pick a random number that lies between 0 and the number of remaining
		// pairs, exclusively.
		T max_index = data->limit * (data->limit - 1);
		#ifndef NDEBUG
		T gaps_before = data->gaps.count();
		#endif
		bool max_index_reached = data->gaps.count() >= max_index;
		bool max_pairs_reached = (data->max_pairs > 0 && data->gaps.count() >= data->max_pairs*2);
		if (max_index_reached || max_pairs_reached) {
			data.reset();
			return;
		}
		std::uniform_int_distribution<T> dis(0, max_index - data->gaps.count() - 1);
		T index = dis(data->rng);

		// Project the integer onto the gap distribution.
		index = data->gaps(index);

		// Split the integer into two.
		current = std::make_pair(
			index / (data->limit - 1),
			index % (data->limit - 1)
		);

		// If the second index points to the first one, increment it by one.
		// This avoids pairing an integer up with itself.
		if (current.first <= current.second)
			++current.second;

		// Since order does not matter, ensure that the first index is always
		// smaller than the second.
		if (current.first > current.second)
			std::swap(current.first, current.second);
		assert(current.second > 0);

		// Punch a gap into the distribution for both orderings of the pair. The
		// increment applied to the second element in case of integers pointing
		// to themselves needs to be compensated here. This means that the value
		// with no stride multiplied to it needs to be reduced by 1 in case it
		// is greater than the other value. Since the first index is guaranteed
		// to be smaller than the second due to the swap above, we can
		// statically subtract 1 in the first case.
		data->gaps.remove(current.first * (data->limit - 1) + current.second - 1);
		data->gaps.remove(current.second * (data->limit - 1) + current.first);
		assert(data->gaps.count() == gaps_before + 2);
	}

	bool is_end() const {
		return !data;
	}

public:
	UniformPairs() {}
	UniformPairs(T limit): UniformPairs(limit, 0) {}
	UniformPairs(T limit, T max_pairs) {
		data.reset(new Inner(limit, max_pairs));
		update();
	}

	pair_t operator*() const {
		return current;
	}

	const pair_t* operator->() const {
		return &current;
	}

	inline bool operator==(const UniformPairs &other) const {
		return is_end() == other.is_end();
	}

	inline bool operator!=(const UniformPairs &other) const {
		return !(*this == other);
	}

	UniformPairs &operator++() {
		update();
		return *this;
	}
};
