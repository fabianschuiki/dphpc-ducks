// Copyright (c) 2018 dphpc-ducks
#pragma once

#include "partial_forest.hpp"
#include <iostream>

bool check_merge(const PartialForest& pf0, const PartialForest& pf1, const PartialForest& exp_merged1into0)
{
	auto merged1into0 = pf0;
	merged1into0.merge(pf1);
	if (merged1into0 == exp_merged1into0) {
		return true;
	} else {
		std::cout << "Mismatch: Merge resulted in " << std::endl
			<< merged1into0 << " but expected " << std::endl
			<< exp_merged1into0 << std::endl;
		return false;
	}
}

bool check_merges(const PartialForest& pf0, const PartialForest& pf1,
		const PartialForest& exp_merge1into0, const PartialForest& exp_merge0into1) {
	return check_merge(pf0, pf1, exp_merge1into0) && check_merge(pf1, pf0, exp_merge0into1);
}
