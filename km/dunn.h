#pragma once
# include "kmeans.h"

enum class inter_ {centroid, closest, furthest, avg}; // distance between clusters
enum class intra_ {closest, furthest, avg }; // distance within cluster

double inter_centroid(vectors centroids, dist_ metric) // smallest distance between centroids 
{
	double min_centroid = distance(centroids, centroids, 0, 1, metric);
	double dist = 0;
	for (int i = 0; i < centroids.n_samples; i++)
	{
		for (int j = i + 1; j < centroids.n_samples; j++)
		{
			dist = distance(centroids, centroids, i, j, metric);
			if (dist < min_centroid)
				min_centroid = dist;
		}
	}
	return min_centroid;
}

double inter_closest(kmeans estim, vectors data)
{
	double min_closest = 0;

	return min_closest;
}

double inter_furthest(kmeans estim, vectors data)
{
	double min_furthest = 0;

	return min_furthest;
}

double inter_avg(kmeans estim, vectors data)
{
	double min_avg = 0;

	return min_avg;
}

double inter_distance(kmeans estim, vectors data, inter_ metric = inter_::centroid)
{
	double inter = 0;
	switch (metric)
	{
	case inter_::centroid:
		inter = inter_centroid(estim.centroids, estim.metric);
		break;
	case inter_::closest:
		inter = inter_closest(estim, data);
		break;
	case inter_::furthest:
		inter = inter_furthest(estim, data);
		break;
	case inter_::avg:
		inter = inter_avg(estim, data);
		break;
	default:
		std::cout << "Invalid Selection for Dunn Index\n";
		exit(1);
	}
	return inter;
}

double intra_closest(vectors labels, vectors data)
{
	double max_closest = 0;

	return max_closest;
}

double intra_furthest(vectors labels, vectors data)
{
	double max_furthest = 0;

	return max_furthest;
}

double intra_avg(kmeans estim, vectors data)
{
	double max_avg = 0;

	return max_avg;
}

double intra_distance(kmeans estim, vectors data, intra_ metric = intra_::avg)
{
	double intra = 0;
	switch (metric)
	{
	case intra_::closest:
		intra = intra_closest(estim.labels, data);
		break;
	case intra_::furthest:
		intra = intra_furthest(estim.labels, data);
		break;
	case intra_::avg:
		intra = intra_avg(estim, data);
		break;
	default:
		std::cout << "Invalid Selection for Dunn Index\n";
		exit(1);
	}
	return intra;
}

double dunn_index(kmeans *estim, vectors data, inter_ metric1 = inter_::centroid, intra_ metric2 = intra_::avg)
{
	if (estim->centroids.n_samples == 1)
	{
		estim->fit(data);
	}
	double inter = inter_distance(*estim, data, metric1);
	double intra = intra_distance(*estim, data, metric2);
	return inter/intra;
}

class DunnSearch 
{
public:
	kmeans estimator;
	intra_ intra;
	inter_ inter;
	int max_clusters;
	int min_clusters;
	double index = 0.0;

	DunnSearch(kmeans est, inter_ inter_d = inter_::centroid, intra_ intra_d = intra_::avg, int min = 2, int max = 10):
		estimator(est),
		inter{inter_d},
		intra{intra_d},
		max_clusters{max},
		min_clusters{min}
	{}

	~DunnSearch()
	{}

	DunnSearch(const DunnSearch& estim):
		estimator(estim.estimator),
		intra{ estim.intra },
		inter{ estim.inter },
		max_clusters{ estim.max_clusters },
		min_clusters{ estim.min_clusters }
	{}

	void fit(vectors data)
	{
		for (int i = min_clusters; i < max_clusters; i++)
		{
		}
	}
};