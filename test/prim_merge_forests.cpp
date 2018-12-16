// Copyright (c) 2018 dphpc-ducks
#include "check_pf_merges.hpp"
#include "prim_msf.hpp"
#include "graphviz.hpp"
#include "pass_fail_tests.hpp"

bool test_single_disjoint_nodes() {
	std::vector<size_t> weights = {1};
	std::vector<std::pair<size_t, size_t>> edges = {
		std::make_pair(0, 1)
	};
	VectorGraph g(edges.begin(), weights.begin(), 2, 1);

	PartialForest pf0(2), pf1(2);
	PrimMsf prim0(g, pf0);
	PrimMsf prim1(g, pf1);
	prim0.run(1, 0);
	prim1.run(1, 1);

	PartialForest exp_1into0 = pf0;
	exp_1into0.add_vertex(1, 0);

	PartialForest exp_0into1 = pf1;
	exp_0into1.add_vertex(0, 1);

	return check_merges(pf0, pf1, exp_1into0, exp_0into1);
}

int main(int argc, char** argv) {

	PassFailTests tests({&test_single_disjoint_nodes});

	std::cout << tests.result() << std::endl;

	return tests.anyFail();
}
