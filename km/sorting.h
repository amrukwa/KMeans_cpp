#pragma once
#include <algorithm>
#include "Vectors.h"

bool descending_sort(int i, int j) 
{ 
	return i > j; 
}

class sort_indices
{
	public:
	vectors idx;
	sort_indices(vectors to_sort): idx(to_sort) {}
	bool operator()(int i, int j) const { return idx.coords[i] > idx.coords[j]; }
};

void sort_two(vectors* basic, vectors* dependent)
// it sorts the columns of dependent one by the values in the basic one - number of values in basic should be equal to the number of cols in dependent
{
	vectors idx = dependent->indices();
	std::sort(idx.coords, idx.coords + idx.n_features * idx.n_samples, sort_indices(*basic));
	std::sort(basic->coords, basic->coords + basic->n_features * basic->n_samples, descending_sort);
}
