#pragma once
# include <ctime>
# include "Vectors.h"


void swap(int* a, int* b)
{
	// function swapping two variables
	int dummy = *a;
	*a = *b;
	*b = dummy;
}

void fisher_yates(int initial_dimensions, int desired_dimensions, int* indices)
{
	// function for obtaining given quantity of non-repeating random numbers in given range
	srand(time(NULL));
	for (int i = 0; i < desired_dimensions; i++) // for the sake of optimalization it should be sufficient
	{
		int a = rand() % initial_dimensions; // index between 1st and last data sample
		swap(&indices[i], &indices[a]);
	}
}

void random_init(vectors* centres, vectors x)
{
	int* indices;
	indices = (int*)malloc(sizeof(int) * x.n_samples);
	for (int i = 0; i < x.n_samples; i++)
	{
		indices[i] = i;
	}
	fisher_yates(x.n_samples, centres->n_samples, indices);
	for (int j = 0; j < centres->n_samples; j++)
	{
		for (int k = 0; k < centres->n_features; k++)
		{
			centres->coords[j * centres->n_features + k] = x.coords[indices[j] * x.n_features + k];
		}
	}
	free(indices);
}

void kpp_init(vectors* centres, vectors x)
{

}

void initialize(vectors* centres, vectors x, std::string init)
{
	if (init == "random")
	{
		random_init(centres, x);
	}

	else if (init == "k++")
	{
		kpp_init(centres, x);
	}
	else
	{
		std::cout << "Unknown initialization type.";
		exit(1);
	}
}