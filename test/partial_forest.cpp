// Copyright (c) 2018 dphpc-ducks
#include "partial_forest.hpp"
#include "check_pf_merges.hpp"

#include <iostream>

bool test_chain_full_overlap() {
	PartialForest pf0(3);
	pf0.add_vertex(0, 0)
		.add_vertex(1, 0)
		.add_vertex(2, 1);

	PartialForest pf1(3);
	pf1.add_vertex(2, 2)
		.add_vertex(1, 2)
		.add_vertex(0, 1);

	return check_merges(pf0, pf1, pf0, pf1);
}

bool test_chain_no_overlap() {
	PartialForest pf0(5);
	pf0.add_vertex(0, 0)
		.add_vertex(1, 0);

	PartialForest pf1(5);
	pf1.add_vertex(4, 4)
		.add_vertex(3, 4)
		.add_vertex(2, 3);

	PartialForest expected(5);
	expected.add_vertex(0, 0)
		.add_vertex(1, 0)
		.add_vertex(4, 4)
		.add_vertex(3, 4)
		.add_vertex(2, 3);

	return check_merges(pf0, pf1, expected, expected);
}

bool test_chain_one_non_root_node_overlap() {
	PartialForest pf0(3);
	pf0.add_vertex(0, 0)
		.add_vertex(1, 0);

	PartialForest pf1(3);
	pf1.add_vertex(2, 2)
		.add_vertex(1, 2);

	PartialForest exp_1into0 = pf0;
	exp_1into0.add_vertex(2, 1);

	PartialForest exp_0into1 = pf1;
	exp_0into1.add_vertex(0, 1);

	return check_merges(pf0, pf1, exp_1into0, exp_0into1);
}

bool test_chain_one_root_node_overlap() {
	PartialForest pf0(3);
	pf0.add_vertex(0, 0)
		.add_vertex(1, 0)
		.add_vertex(2, 1);

	PartialForest pf1(3);
	pf1.add_vertex(2, 2);

	PartialForest exp_1into0 = pf0;

	PartialForest exp_0into1 = pf1;
	exp_0into1.add_vertex(1, 2)
		.add_vertex(0, 1);

	return check_merges(pf0, pf1, exp_1into0, exp_0into1);
}

bool test_chain_two_non_root_nodes_overlap() {
	PartialForest pf0(4);
	pf0.add_vertex(0, 0)
		.add_vertex(1, 0)
		.add_vertex(2, 1);

	PartialForest pf1(4);
	pf1.add_vertex(3, 3)
		.add_vertex(2, 3)
		.add_vertex(1, 2);

	PartialForest exp_1into0 = pf0;
	exp_1into0.add_vertex(3, 2);

	PartialForest exp_0into1 = pf1;
	exp_0into1.add_vertex(0, 1);

	return check_merges(pf0, pf1, exp_1into0, exp_0into1);
}

bool test_chain_two_nodes_incl_root_overlap() {
	PartialForest pf0(4);
	pf0.add_vertex(0, 0)
		.add_vertex(1, 0)
		.add_vertex(2, 1)
		.add_vertex(3, 2);

	PartialForest pf1(4);
	pf1.add_vertex(3, 3)
		.add_vertex(2, 3);

	PartialForest exp_1into0 = pf0;

	PartialForest exp_0into1 = pf1;
	exp_0into1.add_vertex(1, 2)
		.add_vertex(0, 1);

	return check_merges(pf0, pf1, exp_1into0, exp_0into1);
}

bool test_cyclic_graph() {
	PartialForest pf0(4);
	pf0.add_vertex(3, 3);
	pf0.add_vertex(1, 3);
	pf0.add_vertex(0, 1);
	pf0.add_vertex(2, 0);

	PartialForest pf1(4);
	pf1.add_vertex(0, 0);
	pf1.add_vertex(1, 0);
	pf1.add_vertex(2, 0);
	pf1.add_vertex(3, 2);

	return check_merges(pf0, pf1, pf0, pf1);
}

bool test_cyclic_partial_overlap() {
	PartialForest pf0(6);
	pf0.add_vertex(0, 0)
		.add_vertex(1, 0)
		.add_vertex(2, 1)
		.add_vertex(3, 0)
		.add_vertex(4, 3);

	PartialForest pf1(6);
	pf1.add_vertex(5, 5)
		.add_vertex(4, 5)
		.add_vertex(3, 4)
		.add_vertex(0, 3)
		.add_vertex(1, 0);

	PartialForest exp_1into0 = pf0;
	exp_1into0.add_vertex(5, 4);

	PartialForest exp_0into1 = pf1;
	exp_0into1.add_vertex(2, 1);

	return check_merges(pf0, pf1, exp_1into0, exp_0into1);
}

int main(int argc, char **argv) {

	// TODO: Use proper unit testing framework.
	std::vector<bool(*)()> tests = {
		&test_chain_full_overlap,
		&test_chain_no_overlap,
		&test_chain_one_non_root_node_overlap,
		&test_chain_one_root_node_overlap,
		&test_chain_two_non_root_nodes_overlap,
		&test_chain_two_nodes_incl_root_overlap,
		&test_cyclic_graph,
		&test_cyclic_partial_overlap,
	};

	size_t n_fails = 0, n_tests = 0;
	for (auto&& test: tests) {
		if (!test()) {
			std::cout << "Test " << n_tests << " failed!" << std::endl;
			++n_fails;
		}
		++n_tests;
	}
	if (n_fails == 0) {
		std::cout << "All " << n_tests << " tests passed." << std::endl;
		return 0;
	} else {
		std::cout << n_fails << " out of " << n_tests << " tests failed!" << std::endl;
		return 1;
	}
}
