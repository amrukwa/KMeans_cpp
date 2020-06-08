#pragma once
# include "kmeans.h"


class DunnSearch 
{
public:
	kmeans estimator;
	vectors data;
	std::string intra;
	std::string inter;
	int max_clusters;
	int min_clusters;

	DunnSearch(kmeans est, vectors x, std::string inter_d = "centroid", std::string intra_d = "avg", int min = 2, int max = 10):
		estimator(est),
		data(x),
		inter{inter_d},
		intra{intra_d},
		max_clusters{max},
		min_clusters{min}
	{}

	~DunnSearch()
	{}

	DunnSearch(const DunnSearch& estim):
		estimator(estim.estimator),
		data(estim.data),
		intra{ estim.intra },
		inter{ estim.inter },
		max_clusters{ estim.max_clusters },
		min_clusters{ estim.min_clusters }
	{}
};