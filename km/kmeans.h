#pragma once
# include "initialization.h"

void label_points(vectors* labels, vectors x, vectors centroids, dist_ metric)
// this function changes labels based on the distance of each corresponding sample of x to the samples of centroids in distance metric
{
	for (int i = 0; i < x.n_samples; i++)
	{
		labels->coords[i] = argmin_distance(x, centroids, i, metric);
	}
}

void calculate_centroids(vectors labels, vectors x, vectors* centroids)
// this function calculates centroids based on mean of coords in x of samples with the same labels coords
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

double calculate_inertia(vectors labels, vectors x, vectors centroids, dist_ metric)
// this function calculates inertia of vectors x based on their labels and distance to the appropriate sample of centroids
{
	double inertia = 0;
	double dist = 0;
	for (int i = 0; i < x.n_samples; i++)
	{
		dist = distance(x, centroids, i, labels.coords[i], metric);
		inertia += dist * dist;
	}
	return inertia;
}

void kmeans_algorithm(vectors* centroids,
	vectors* labels,
	dist_ metric,
	int n_clusters,
	init_ initialization,
	double* inertia,
	int max_iter,
	int* n_iter,
	vectors x)
// this function performs kmeans clustering on given vectors x
{
	vectors prev_labels(1, x.n_samples);
	if (centroids->n_samples > x.n_samples)
	{
		std::cout << "Too many clusters desired.";
		exit(1);
	}
	initialize(centroids, x, initialization, metric);
	label_points(labels, x, *centroids, metric);
	for (*n_iter; *n_iter < max_iter; (*n_iter)++)
	{
		prev_labels = *labels;
		calculate_centroids(*labels, x, centroids);
		label_points(labels, x, *centroids, metric);
		if (prev_labels == *labels)
			break;
	}
	*inertia = calculate_inertia(*labels, x, *centroids, metric);
}

class kmeans 
// this class is a classifier performing k-means clustering based on specified parameters
{
public:
	vectors centroids; // Coordinates of cluster centers and their dimensions
	dist_ metric; // Measure of distance between points; one of dist_: Euclidean, correlation or cityblock
	int n_clusters; // The number of clusters to form as well as the number of centroids to generate, should be int greater than 1
	init_ initialization; // Method for initialization; one of init_: random or kpp
	double inertia = 0; // Sum of squared distances of samples to their closest cluster center
	int max_iter; // Maximum number of iterations of the k-means algorithm for a single run
	int n_iter = 0; // Number of iterations run
	vectors labels; // Labels of each point
	int n_init; // Number of time the k-means algorithm will be run. The final results will be the best output of n_init consecutive runs in terms of inertia

	kmeans(int clusters_n, dist_ metrics = dist_::correlation, init_ init = init_::kpp, int iter = 1000, int init_n = 10) :
	// constructor setting all attributes for future clustering
		centroids(1, 1),
		labels(1, 1),
		metric{ metrics },
		n_clusters{ clusters_n },
		initialization{ init },
		max_iter{ iter },
		n_init{init_n}
	{}

	kmeans() = default;
	// default constructor; usage not encouraged

	~kmeans() {}
	// destructor

	kmeans(const kmeans& estim) :
	// copy constructor: creates a deep copy of estim 
		metric{ estim.metric },
		n_clusters{ estim.n_clusters },
		initialization{ estim.initialization },
		max_iter{ estim.max_iter },
		labels(estim.labels),
		centroids(estim.centroids),
		n_init{estim.n_init},
		inertia{ estim.inertia },
		n_iter{ estim.n_iter }
	{}

	kmeans(const kmeans& estim, int clusters_n) :
	// copy constructor for getting the same parameters but different number of clusters 
	// the object should be fit before  predicting labels, as centroids are not copied
		metric{ estim.metric },
		n_clusters{ clusters_n },
		initialization{ estim.initialization },
		max_iter{ estim.max_iter },
		labels(1, 1),
		centroids(1, 1),
		inertia{estim.inertia},
		n_iter{estim.n_iter},
		n_init{ estim.n_init }
	{}

	kmeans& operator=(const kmeans& estim)
	// deep assignment operator
	// it assigns values behind the estimator, not only the pointers
	{
		metric = estim.metric;
		n_clusters = estim.n_clusters;
		initialization = estim.initialization;
		centroids = estim.centroids;
		labels = estim.labels;
		max_iter = estim.max_iter;
		n_init = estim.n_init;
		n_iter = estim.n_iter;
		inertia = estim.inertia;
		return *this;
	}

	void fit(vectors data)
	// Compute k-means clustering
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
	// Predict the closest cluster each sample in data belongs to
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
	// convenience method
	// equal to calling fit and then predict on the same data
	{
		fit(data);
		return labels;
	}
};
