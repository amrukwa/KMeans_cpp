# include <iostream>
#include<fstream>
# include <cstdlib>
#pragma once


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

	vectors(std::ifstream& datafile)
	{
		get_dimensions(datafile);
		coords = (double*)malloc(n_samples*n_features*sizeof(double));
		datafile.clear();
		datafile.seekg(0, std::ios::beg);
		load_data(datafile);
	}

	~vectors()
	{}

	void get_dimensions(std::ifstream& datafile)
	{
		char c;
		while (datafile.get(c))
		{
			if (c == '\n')
			{
				n_samples += 1;
				n_features += 1;
			}
			else if (c == ' ')
			{
				n_features += 1;
			}
		}
		n_features /= n_samples;
	}

	void load_data(std::ifstream& datafile)
	{
		double c=1;
		for (int i = 0; i < n_samples; i++)
		{
			for (int j = 0; j < n_features; j++)
			{
				datafile >> coords[i*n_features+j];
			}
		}
	}

	void shape()
	{
		std::cout << "("<<n_samples << "," << n_features<<")" << std::endl;
	}
};
