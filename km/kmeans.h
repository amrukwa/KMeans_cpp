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
	int n_iter;

	kmeans(int clusters_n, std::string metrics = "Euclidean", std::string init = "random", int iter = 1000)
	{
		n_clusters = clusters_n;
		metric = metrics;
		initialization = init;
		max_iter = iter;
		n_iter = 0;
		inertia = 0;
	}

	~kmeans() {}

	kmeans(const kmeans& estim) :
		metric{ estim.metric },
		n_clusters{ estim.n_clusters },
		initialization{ estim.initialization },
		inertia{ 0 },
		max_iter{ estim.max_iter },
		n_iter{ 0 },
		centroids(estim.centroids)
	{}

	void fit(vectors data)
	{

	}
private:
	vectors labels;
};