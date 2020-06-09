#pragma once
# include "kmeans.h"

enum class inter_ {centroid, closest, furthest, avg};
enum class intra_ {closest, furthest, avg };

double inter_centroid(kmeans estim)
{
	double min_centroid = 0;

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

double dunn_index(kmeans *estim, vectors data, inter_ metric1 = inter_::centroid, intra_ metric2 = intra_::avg)
{
	double inter = 0;
	double intra = 0;
	if (estim->centroids.n_samples == 1)
	{
		estim->fit(data);
	}
	switch (metric1)
	{
	case inter_::centroid:
		inter = inter_centroid(*estim);
		break;
	case inter_::closest:
		inter = inter_closest(*estim, data);
		break;
	case inter_::furthest:
		inter = inter_furthest(*estim, data);
		break;
	case inter_::avg:
		inter = inter_avg(*estim, data);
		break;
	default:
		std::cout << "Invalid Selection for Dunn Index\n";
		exit(1);
	}
	switch (metric2)
	{
	case intra_::closest:
		intra = intra_closest(estim->labels, data);
		break;
	case intra_::furthest:
		intra = intra_furthest(estim->labels, data);
		break;
	case intra_::avg:
		intra = intra_avg(*estim, data);
		break;
	default:
		std::cout << "Invalid Selection for Dunn Index\n";
		exit(1);
	}
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