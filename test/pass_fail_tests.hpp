// Copyright (c) 2018 dphpc-ducks
#pragma once

#include <boost/dynamic_bitset.hpp>
#include <sstream>
#include <vector>

class PassFailTests {
public:
	typedef bool test_fn_t();

private:
	std::vector<test_fn_t*> tests;
	boost::dynamic_bitset<> tests_not_run, tests_failed;

	bool run(size_t test_id) {
		assert(test_id < tests.size());
		const bool passed = tests[test_id]();
		if (!passed)
			tests_failed.set(test_id);
		tests_not_run.reset(test_id);
		return passed;
	}

public:
	PassFailTests(std::vector<test_fn_t*> tests): tests(tests) {
		tests_not_run.resize(tests.size(), true);
		tests_failed.resize(tests.size(), false);
	}

	void runAll() {
		while (tests_not_run.any()) {
			run(tests_not_run.find_first());
		}
	}

	bool anyFail() {
		runAll();
		return tests_failed.any();
	}

	std::string result() {
		std::ostringstream os;
		if (anyFail()) {
			if (tests_failed.count() > 1) {
				os << "Tests ";
				const auto first_fail = tests_failed.find_first();
				os << first_fail;
				for (size_t i = first_fail+1; i < tests_failed.size(); ++i) {
					if (tests_failed[i])
						os << ", " << i;
				}
			} else {
				os << "Test " << tests_failed.find_first();
			}
			os << " failed!";
		} else
			os << "All " << tests.size() << " tests passed.";
		return os.str();
	}

};
