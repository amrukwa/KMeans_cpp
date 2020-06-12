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
	double dist = 0;
	int count = 0;
	for (int i = 0; i < data.n_samples - 1; i++)
	{
		for (int j = i + 1; j < data.n_samples; j++)
		{
			if (labels.coords[i] == c1 && labels.coords[j] == c2)
			{
				dist += distance(data, data, i, j, metric);
				count += 1;
			}
		}
	}
	if (count == 0) // this can happen only for avg distance within cluster for one member - as we are looking for the biggest distance in intra distances, it will be skipped
	{
		return -1;
	}
	return dist/count;
}

double inter_linkage(vectors labels, vectors data, int c1, int c2, dist_ metric, inter_ link)
{
	double dist = 0;
	if (link == inter_::closest)
		dist = single_linkage(labels, data, c1, c2, metric);
	else if (link == inter_::furthest)
		dist = complete_linkage(labels, data, c1, c2, metric);
	else if (link == inter_::avg)
		dist = avg_linkage(labels, data, c1, c2, metric);
	else
	{
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

double intra_centroid(kmeans* est, vectors data, int c)
{
	double dist = 0;
	int count = 0;
	for (int i = 0; i < data.n_samples; i++)
	{
		if (est->labels.coords[i] == c)
		{
			dist += distance(data, est->centroids, i, c, est->metric);
			count += 1;
		}
	}
	return dist/count;
}

double intra_linkage(kmeans* estim, vectors data, int c, intra_ metric)
{
	double intra = 0;
	if (metric == intra_::centroid)
		intra = intra_centroid(estim, data, c);
	else if(metric == intra_::furthest)
		intra = complete_linkage(estim->labels, data, c, c, estim->metric);
	else if(metric == intra_::avg)
		intra = avg_linkage(estim->labels, data, c, c, estim->metric);
	else
	{
		std::cout << "Invalid Selection for Dunn Index\n";
		exit(1);
	}
	return intra;
}

double intra_distance(kmeans* est, vectors data, intra_ metric= intra_::avg)
{
	double cur, dist = intra_linkage(est, data, 0, metric);
	for (int i = 0; i < est->n_clusters; i++)
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
	int max_clusters=10;
	int min_clusters=2;
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
		min_clusters{ estim.min_clusters },
		index{estim.index}
	{}

	double single_idx(kmeans* est, vectors data, int clusters_n)
	{
		if (est->n_clusters != clusters_n)
			est->n_clusters = clusters_n;
		est->fit(data);
		return dunn_index(est, data, inter, intra);
	}

	void fit(vectors data)
	{
		double idx;
		kmeans temp;
		for (int i = min_clusters; i <= max_clusters; i++)
		{
			temp = estimator;
			idx = single_idx(&temp, data, i);
			if (idx > index)
			{
				estimator = temp;
				index = idx;
			}
		}
	}
};