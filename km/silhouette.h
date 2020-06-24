#pragma once
# include "kmeans.h"
# include <algorithm>

double avg_to_cluster(vectors labels, vectors data, int sample, int c, dist_ metric)
// calculates the average distance of the sample in data to samples in cluster c in metric 
{
	double dist = 0;
	int count = 0;
	for (int i = 0; i < data.n_samples; i++)
	{
		if (i == sample || labels.coords[i] != c)
			continue;
		count += 1;
		dist += distance(data, data, sample, i, metric);
	}
	if (count == 0)
		return 0;
	return dist/count;
}

double min_avg(vectors labels, vectors data, int sample, int n_clusters, dist_ metric)
// calculates the distance of the sample in data to closest cluster it doesn't belong to, in metric
{
	double cur, min_dist = LONG_MAX;
	for (int i = 0; i < n_clusters; i++)
	{
		if (i == labels.coords[sample])
			continue;
		cur = avg_to_cluster(labels, data, sample, i, metric);
		if (cur < min_dist)
			min_dist = cur;
	}
	return min_dist;
}

double _for_sample(kmeans* est, vectors data, int sample)
// calculates Silhouette score for specified sample in data
{
	double a= avg_to_cluster(est->labels, data, sample, est->labels.coords[sample], est->metric);
	double b = min_avg(est->labels, data, sample, est->n_clusters, est->metric);
	double den = std::max(a, b);
	return (b-a)/den;
}

double silhouette(kmeans* est, vectors data)
// calculates average Silhouette score for data after clustering
{
	if (est->centroids.n_samples == 1)
		est->fit(data);
	double coef = 0;
	for (int i = 0; i < data.n_samples; i++)
	{
		coef += _for_sample(est, data, i);
	}
	return coef / data.n_samples;
}

class SilhouetteSearch
// choose the best number of clusters for k-means with Silhouette Score
{
public:
	kmeans estimator;  // estimator of the greatest Silhouette Score from min_clusters to max_clusters interval
	int max_clusters = 20; // biggest number of clusters for estimator
	int min_clusters = 2; // smallest number of clusters for estimator
	double coefficient = 0.0; // Silhouette Score value for estimator

	SilhouetteSearch(kmeans est, int min = 2, int max = 20) :
		estimator(est),
		max_clusters{ max },
		min_clusters{ min }
		// constructor
		// makes a deep copy of the given estimator
	{}

	~SilhouetteSearch()
	// destructor
	{}

	SilhouetteSearch(const SilhouetteSearch& estim) :
		estimator(estim.estimator),
		max_clusters{ estim.max_clusters },
		min_clusters{ estim.min_clusters },
		coefficient{ estim.coefficient }
	// copy constructor
	// creates a deep copy of the other object
	{}

	double single_coefficient(kmeans* est, vectors data, int clusters_n)
	// calculates a single Silhouette Score for *est
	// automatically fits *est and changes its number of clusters
	{
		if (est->n_clusters != clusters_n)
			est->n_clusters = clusters_n;
		est->fit(data);
		return silhouette(est, data);
	}

	void fit(vectors data)
	// performs k-means clustering for kmeans estimators with n_cluster between max_clusters and min_clusters
	// chooses the estimator with the highest Silhouette Score
	{
		double coef;
		kmeans temp;
		for (int i = min_clusters; i <= max_clusters; i++)
		{
			temp = estimator;
			coef = single_coefficient(&temp, data, i);
			if (coef > coefficient)
			{
				estimator = temp;
				coefficient = coef;
			}
		}
	}
};