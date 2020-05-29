#pragma once
# include "initialization.h"

class kmeans {
public:
	vectors centroids;
	std::string metric;
	int n_clusters;
	std::string initialization;
	double inertia;
	int max_iter;
	int n_iter;
	vectors labels;

	kmeans(int clusters_n, std::string metrics = "Euclidean", std::string init = "random", int iter = 1000) :
		centroids(1, 1),
		labels(1, 1),
		metric{ metrics },
		n_clusters{ clusters_n },
		initialization{ init },
		inertia{ 0 },
		max_iter{ iter },
		n_iter{0}
	{}

	~kmeans() {}

	kmeans(const kmeans& estim) :
		metric{ estim.metric },
		n_clusters{ estim.n_clusters },
		initialization{ estim.initialization },
		inertia{ 0 },
		max_iter{ estim.max_iter },
		n_iter{ 0 },
		labels(1, 1),
		centroids(estim.centroids)
	{}

	void fit(vectors data)
	{
		labels.change_size(1, data.n_samples);
		centroids.change_size(n_clusters, data.n_features);
		if (centroids.n_samples > data.n_samples)
		{
			std::cout << "Too many clusters desired.";
			exit(1);
		}
		initialize(&centroids, data, initialization, metric);
	}

	vectors predict(vectors data)
	{
		if (data.n_features != centroids.n_features)
		{
			std::cout << "Incorrect dimensionality";
			exit(1);
		}
		return labels;
	}

	vectors fit_predict(vectors data)
	{
		fit(data);
		return labels;
	}
};