#pragma once
# include "initialization.h"

void label_points(vectors* labels, vectors x, vectors centroids, std::string metric);
void calculate_centroids(vectors labels, vectors x, vectors* centroids);
double calculate_inertia(vectors labels, vectors x, vectors centroids, std::string metric);
void kmeans_algorithm(vectors* centroids,
	vectors* labels,
	std::string metric,
	int n_clusters,
	std::string initialization,
	double* inertia,
	int max_iter,
	int* n_iter,
	vectors x);

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
		n_init{init_n}
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

	void fit(vectors data)
	{
		labels.change_size(1, data.n_samples);
		centroids.change_size(n_clusters, data.n_features);
		vectors cur_cent(n_clusters, data.n_features);
		vectors cur_l(1, data.n_samples);
		double cur_inertia = 0;
		int cur_iter = 0;
		kmeans_algorithm(&centroids,
			&labels,
			metric,
			n_clusters,
			initialization,
			&inertia,
			max_iter,
			&n_iter,
			data);
		for (int i = 1; i < n_init; i++)
		{
			kmeans_algorithm(&cur_cent,
				&cur_l,
				metric,
				n_clusters,
				initialization,
				&cur_inertia,
				max_iter,
				&cur_iter,
				data);
			if (cur_inertia < inertia)
			{
				inertia = cur_inertia;
				n_iter = cur_iter;
				labels = cur_l;
				centroids = cur_cent;
			}
		}
	}

	vectors predict(vectors data)
	{
		if (data.n_features != centroids.n_features)
		{
			std::cout << "Incorrect dimensionality";
			exit(1);
		}
		if (data.n_samples != labels.n_features)
		{
			labels.change_size(1, data.n_samples);
		}
		label_points(&labels, data, centroids, metric);
		inertia = calculate_inertia(labels, data, centroids, metric);
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

double calculate_inertia(vectors labels, vectors x, vectors centroids, std::string metric)
{
	double inertia = 0;
	double dist = 0;
	for (int i = 0; i < x.n_samples; i++)
	{
		dist = distance(x, centroids, i, labels.coords[i], metric);
		inertia += dist*dist;
	}
	return inertia;
}

void kmeans_algorithm(vectors* centroids, 
	vectors* labels,
	std::string metric,
	int n_clusters,
	std::string initialization,
	double* inertia,
	int max_iter,
	int* n_iter,
	vectors x)
{
	vectors prev_labels(1, x.n_samples);
	if (centroids->n_samples > x.n_samples)
	{
		std::cout << "Too many clusters desired.";
		exit(1);
	}
	initialize(centroids, x, initialization, metric);
	label_points(labels, x, *centroids, metric);
	*n_iter = 0;
	for (*n_iter; *n_iter < max_iter; (*n_iter)++)
	{
		for (int i = 0; i < labels->n_features; i++)
		{
			prev_labels.coords[i] = labels->coords[i];
		}
		calculate_centroids(*labels, x, centroids);
		label_points(labels, x, *centroids, metric);
		if (prev_labels == *labels)
		{
			break;
		}
	}
	*inertia = calculate_inertia(*labels, x, *centroids, metric);
}
