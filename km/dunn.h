#pragma once
# include "kmeans.h"

enum class inter_ {centroid, closest, furthest, avg}; // distance between clusters
enum class intra_ {centroid, furthest, avg }; // distance within cluster


double single_linkage(vectors labels, vectors data, int c1, int c2, dist_ metric)
// closest distance between two samples belonging to two given clusters
{
	double cur, min_dist = LONG_MAX;
	for (int i = 0; i < data.n_samples-1; i++)
	{
		for (int j = i + 1; j < data.n_samples; j++)
		{
			if (labels.coords[i] == c1 && labels.coords[j] == c2)
			{
				cur = distance(data, data, i, j, metric);
				if (cur < min_dist)
					min_dist = cur;
			}
		}
	}
	return min_dist;
}

double complete_linkage(vectors labels, vectors data, int c1, int c2, dist_ metric)
// the distance between the most remote samples belonging to two given clusters
{
	double cur, min_dist = 0;
	for (int i = 0; i < data.n_samples-1; i++)
	{
		for (int j = i + 1; j < data.n_samples; j++)
		{
			if (labels.coords[i] == c1 && labels.coords[j] == c2)
			{
				cur = distance(data, data, i, j, metric);
				if (cur > min_dist)
					min_dist = cur;
			}
		}
	}
	return min_dist;
}

double avg_linkage(vectors labels, vectors data, int c1, int c2, dist_ metric)
{
	return 0;
}

double inter_linkage(vectors labels, vectors data, int c1, int c2, dist_ metric, inter_ link)
{
	double dist = 0;
	switch (link)
	{
	case inter_::closest:
		dist = single_linkage(labels, data, c1, c2, metric);
		break;
	case inter_::furthest:
		dist = complete_linkage(labels, data, c1, c2, metric);
		break;
	case inter_::avg:
		dist = avg_linkage(labels, data, c1, c2, metric);
		break;
	default:
		std::cout << "Invalid Selection for Dunn Index\n";
		exit(1);
	}
	return dist;
}

double inter_dist(kmeans* est, vectors data, inter_ metric)
{
	double cur, dist = inter_linkage(est->labels, data, 0, 1, est->metric, metric);
	for (int i = 0; i < est->n_clusters - 1; i++)
	{
		for (int j = i + 1; j < est->n_clusters; j++)
		{
			cur = inter_linkage(est->labels, data, i, j, est->metric, metric);
			if (cur < dist)
				dist = cur;
		}
	}
	return dist;
}

double inter_centroid(vectors centroids, dist_ metric) // smallest distance between centroids 
{
	double dist, min_centroid = distance(centroids, centroids, 0, 1, metric);
	for (int i = 0; i < centroids.n_samples-1; i++)
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

double inter_distance(kmeans *estim, vectors data, inter_ metric = inter_::centroid)
{
	double inter = 0;
	switch (metric)
	{
	case inter_::centroid:
		inter = inter_centroid(estim->centroids, estim->metric);
		break;
	default:
		inter = inter_dist(estim, data, metric);
		break;
	}
	return inter;
}

double intra_centroid()
{
	return 0;
}

double intra_furthest(vectors labels, vectors data, dist_ metric, int n_clusters)
{
	double cur, dist = complete_linkage(labels, data, 0, 0, metric);
	for (int i = 1; i < n_clusters; i++)
	{
		cur = complete_linkage(labels, data, i, i, metric);
		if (cur > dist)
			dist = cur;
	}
	return dist;
}

double intra_avg(kmeans estim, vectors data)
{
	double max_avg = 0;

	return max_avg;
}

double intra_linkage(kmeans* estim, vectors data, int c, intra_ metric = intra_::avg)
{
	double intra = 0;
	switch (metric)
	{
	case intra_::centroid:
		intra = intra_centroid();
		break;
	case intra_::furthest:
		intra = complete_linkage(estim->labels, data, c, c, estim->metric);
		break;
	case intra_::avg:
		intra = intra_avg(*estim, data);
		break;
	default:
		std::cout << "Invalid Selection for Dunn Index\n";
		exit(1);
	}
	return intra;
}

double intra_distance(kmeans* est, vectors data, intra_ metric= intra_::avg)
{
	double cur, dist = intra_linkage(est, data, 0, metric);
	for (int i = 0; i < est->n_clusters - 1; i++)
	{
		cur = intra_linkage(est, data, i, metric);
		if (cur > dist)
			dist = cur;
	}
	return dist;
}

double dunn_index(kmeans *estim, vectors data, inter_ metric1 = inter_::centroid, intra_ metric2 = intra_::avg)
{
	if (estim->centroids.n_samples == 1)
	{
		estim->fit(data);
	}
	double inter = inter_distance(estim, data, metric1);
	double intra = intra_distance(estim, data, metric2);
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