#pragma once
# include "initialization.h"

void label_points(vectors* labels, vectors x, vectors centroids, std::string metric);
void calculate_centroids(vectors labels, vectors x, vectors* centroids);

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
	int n_init;

	kmeans(int clusters_n, std::string metrics = "correlation", std::string init = "k++", int iter = 1000, int init_n = 10) :
		centroids(1, 1),
		labels(1, 1),
		metric{ metrics },
		n_clusters{ clusters_n },
		initialization{ init },
		inertia{ 0 },
		max_iter{ iter },
		n_iter{0},
		n_init{ init_n }
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
		centroids(estim.centroids),
		n_init{estim.n_init}
	{}

	void calculate_inertia(vectors data)
	{
	}

	void fit(vectors data)
	{
		vectors prev_labels(data.n_samples, 1);
		labels.change_size(data.n_samples, 1);
		centroids.change_size(n_clusters, data.n_features);
		if (centroids.n_samples > data.n_samples)
		{
			std::cout << "Too many clusters desired.";
			exit(1);
		}
		initialize(&centroids, data, initialization, metric);
		label_points(&labels, data, centroids, metric);
		for (n_iter; n_iter < max_iter; n_iter++)
		{
			for (int i = 0; i < labels.n_samples; i++)
			{
				prev_labels.coords[i] = labels.coords[i];
			}
			calculate_centroids(labels, data, &centroids);
			label_points(&labels, data, centroids, metric);
			if  (prev_labels == labels)
			{
				break;
			}
		}
		calculate_inertia(data);
	}

	vectors predict(vectors data)
	{
		if (data.n_features != centroids.n_features)
		{
			std::cout << "Incorrect dimensionality";
			exit(1);
		}
		if (data.n_samples != labels.n_samples)
		{
			labels.change_size(data.n_samples, 1);
		}
		label_points(&labels, data, centroids, metric);
		calculate_inertia(data);
		return labels;
	}

	vectors fit_predict(vectors data)
	{
		fit(data);
		return labels;
	}
};


void label_points(vectors* labels, vectors x, vectors centroids, std::string metric)
{
	for (int i = 0; i < x.n_samples; i++)
	{
		labels->coords[i] = argmin_distance(x, centroids, i, metric);
	}
}

void calculate_centroids(vectors labels, vectors x, vectors* centroids)
{
	double sum_on_axis;
	int members;
	for (int cur_cent = 0; cur_cent < centroids->n_samples; cur_cent++)
	{
		for (int cur_dim = 0; cur_dim < centroids->n_features; cur_dim++)
		{
			sum_on_axis = 0;
			members = 0;
			for (int cur_point = 0; cur_point < x.n_samples; cur_point++)
			{
				if (labels.coords[cur_point] == cur_cent)
				{
					++members;
					sum_on_axis += x.coords[cur_point * x.n_features + cur_dim];
				}
				centroids->coords[cur_cent * centroids->n_features + cur_dim] = sum_on_axis / members;
			}
		}
	}
}

void fit_n()
{
}