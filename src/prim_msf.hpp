// Copyright (c) 2018 dphpc-ducks
#pragma once
#include "partial_forest.hpp"
#include "vector_graph.hpp"

void prim_minimum_spanning_forest(const VectorGraph &g, PartialForest &msf, size_t n_steps, size_t offset);
