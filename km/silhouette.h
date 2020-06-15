#pragma once
# include "kmeans.h"

double avg_to_cluster(vectors labels, vectors data, int sample, int c, dist_ metric)
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

double _for_sample(kmeans* est, vectors data, int sample)
{
	double a, b;

	double den = std::max(a, b);
	return 0;
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


	double single_coefficient(kmeans* est, vectors data, int clusters_n)
	{
		if (est->n_clusters != clusters_n)
			est->n_clusters = clusters_n;
		est->fit(data);
		double coef = 0;
		//
		return coef/data.n_samples;
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