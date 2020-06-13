#pragma once
# include "kmeans.h"

vectors distance_matrix(vectors x, dist_ metric = dist_::Euclidean)
{
	vectors temp(x.n_samples, x.n_samples);
	for (int i = 0; i < x.n_samples; i++)
	{
		for (int j = 0; j < x.n_samples; j++)
		{
			temp.coords[i * temp.n_features + j] = distance(x, x, i, j, metric);
		}
	}

	return temp;
}

class SilhouetteSearch
{
public:
	kmeans estimator;
	int max_clusters = 10;
	int min_clusters = 2;
	double coefficient = 0.0;

	SilhouetteSearch(kmeans est, int min = 2, int max = 10) :
		estimator(est),
		max_clusters{ max },
		min_clusters{ min }
	{}

	~SilhouetteSearch()
	{}

	SilhouetteSearch(const SilhouetteSearch& estim) :
		estimator(estim.estimator),
		max_clusters{ estim.max_clusters },
		min_clusters{ estim.min_clusters },
		coefficient{ estim.coefficient }
	{}

	double silhouette_for_sample(kmeans* est, vectors data)
	{
		return 0;
	}

	double single_coefficient(kmeans* est, vectors data, int clusters_n)
	{
		if (est->n_clusters != clusters_n)
			est->n_clusters = clusters_n;
		est->fit(data);
		return 0;
	}

	void fit(vectors data)
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