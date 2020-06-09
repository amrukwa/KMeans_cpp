#pragma once
# include "kmeans.h"

double dunn_index(kmeans estim, vectors data)
{
	double dunn = 0;
	return dunn;
}

class DunnSearch 
{
public:
	kmeans estimator;
	std::string intra;
	std::string inter;
	int max_clusters;
	int min_clusters;


	DunnSearch(kmeans est, std::string inter_d = "centroid", std::string intra_d = "avg", int min = 2, int max = 10):
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