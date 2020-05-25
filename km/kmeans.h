#pragma once
# include "Vectors.h"

class kmeans {
public:
	vectors centroids;
	std::string metric;
	int n_clusters;
	std::string initialization;
	double inertia;
	int max_iter;
	kmeans()
	{

	}
private:
	vectors labels;
};