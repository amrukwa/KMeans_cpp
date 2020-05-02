# include <iostream>
# include <cstdlib>
#pragma once

class vectors
{
public:
	int n_samples;
	int n_features;
	double* coords;

	vectors() 
	{

	}

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
	{

	}


};
