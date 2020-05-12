# include <iostream>
#include<fstream>
# include <cstdlib>
#pragma once

void get_dimensions(int* n_features, int* n_samples, std::ifstream& datafile);

void add_space(char c, int* n_features, int * n_samples);

class vectors
{
public:
	int n_samples = 0;
	int n_features = 0;
	double* coords;

	vectors() = default;

	vectors(int samples, int features, double* data)
	{
		n_features = features;
		n_samples = samples;
		coords = data;
	}

	vectors(const vectors& v1)
	{
		n_samples = v1.n_samples;
		n_features = v1.n_features;
		coords = v1.coords;
	}

	~vectors()
	{}

	void genfromtxt(std::ifstream &datafile)
	{
		get_dimensions(&n_features, &n_samples, datafile);
		coords = (double*)malloc(n_features* n_samples * sizeof(double));
	 }
};

void get_dimensions(int* n_features, int* n_samples, std::ifstream &datafile)
{
	char c;
	if (!datafile)
	{
		std::cout << "Cannot open file.\n";
		exit(1);
	}
	datafile >> std::noskipws >> c;
	while (c != EOF)
	{
		add_space(c, n_features, n_samples);
		datafile >> std::noskipws >> c;
	}
	*n_features = *n_features / *n_samples;
	datafile.seekg(0);
}

void add_space(char c, int* n_features, int* n_samples)
{
	if (c == ' ')
	{
		*n_features += *n_features;
	}
	else if (c == '\n')
	{
		*n_features += *n_features;
		*n_samples += *n_samples;
	}
}