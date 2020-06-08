#pragma once
# include <ctime>
# include "Vectors.h"

void fisher_yates(int desired_dimensions, vectors* ind)
{
	// function for obtaining given quantity of non-repeating random numbers in given range
	srand(time(NULL));
	for (int i = 0; i < desired_dimensions; i++) // for the sake of optimalization it should be sufficient
	{
		int a = rand() % ind->n_features;
		// index between 1st and last data sample
		swap(&ind->coords[i], &ind->coords[a]);
	}
}

void random_init(vectors* centres, vectors x)
{
	vectors ind = indices(x.n_samples);
	fisher_yates(centres->n_samples, &ind);
	for (int j = 0; j < centres->n_samples; j++)
	{
		for (int k = 0; k < centres->n_features; k++)
		{
			centres->coords[j * centres->n_features + k] = x.coords[int(ind.coords[j]) * x.n_features + k];
		}
	}
}

void first_centroid(vectors* centres, vectors x)
{
	srand(time(NULL));
	int a = rand() % x.n_samples;
	for (int i = 0; i < x.n_features; i++)
	{
		centres->coords[i] = x.coords[a * x.n_features + i];
	}
}

int weighted_random(vectors weights)
{
	int index;
	srand(time(NULL));
	double val = (double)rand() / RAND_MAX;
	double v = 0;
	for (index = 0; index < weights.n_samples; index++)
	{
		v += weights.coords[index];
		if (val < v)
		{
			break;
		}
	}
	return index;
}

void next_centroid(vectors* centres, vectors x, vectors* weights, int c_index, std::string metric)
{
	// c_index is the index where next centroid will be appended
	double distance;
	for (int j = 0; j < x.n_samples; j++) // calculate the dist of vector to the closest centre and square it
	{
		weights->coords[j] = min_distance(x, *centres, j, c_index, metric);
		weights->coords[j] = weights->coords[j] * weights->coords[j];
	}
	weights->divide( weights->sum());
	int idx = weighted_random(*weights);
	for (int i = 0; i < centres->n_features; i++)
	{
		centres->coords[c_index * centres->n_features + i] = x.coords[idx*x.n_features + i];
	}
}

void kpp_init(vectors* centres, vectors x, std::string metric)
{
	vectors weights(x.n_samples, 1);
	first_centroid(centres, x);
	for (int i = 1; i < centres->n_samples; i++)
	{
		next_centroid(centres, x, &weights, i, metric);
	}
}

void initialize(vectors* centres, vectors x, std::string init, std::string metric)
{
	if (init == "random")
	{
		random_init(centres, x);
	}

	else if (init == "k++")
	{
		kpp_init(centres, x, metric);
	}
	else
	{
		std::cout << "Unknown initialization type.";
		exit(1);
	}
}