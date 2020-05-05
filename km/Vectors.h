# include <iostream>
#include<fstream>
# include <cstdlib>
#pragma once

void get_dimensions(int* n_features, int* n_samples, std::ifstream& datafile);

class vectors
{
public:
	int n_samples;
	int n_features;
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
	 }
};

void get_dimensions(int* n_features, int* n_samples, std::ifstream &datafile)
{
	int x = 0;
	int y = 0;
	char c;
	if (!datafile)
	{
		std::cout << "Cannot open file.\n";
		exit(1);
	}
	datafile >> std::noskipws >> c;
	while (c != EOF)
	{
		if (c == ' ')
		{
			x++;
		}
		else if (c == '\n')
		{
			x++;
			y++;
		}
		char b = c;
		datafile >> std::noskipws >> c;
	}
}