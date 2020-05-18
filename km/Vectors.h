# include <iostream>
#include<fstream>
# include <cstdlib>
# include <string>
#include <math.h>
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

	vectors(int square_dim)
	{
		n_features = square_dim;
		n_samples = square_dim;
		coords = (double*)malloc(n_samples * n_features * sizeof(double));
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

	double sum_of_column(int column)
	{
		if (column >= n_samples)
		{
			std::cout << "Invalid index.";
			exit(1);
		}
		double sum = 0;
		for (int i = 0; i < n_samples; i++)
		{
			sum += coords[i* n_features + column];
		}
		return sum;
	}

	void mean(int column, double* mean) 
	{
		double s = sum_of_column(column);
		*mean =  s / n_samples;
	}

	void substract(double value)
	{
		for (int i = 0; i < n_features * n_samples; i++)
		{
			coords[i] -= value;
		}
	}

	void substract(double value, int column)
	{
		for (int i = 0; i < n_samples; i++)
		{
			coords[i * n_features + column] -= value;
		}
	}

	void divide(double value, int column)
	{
		if (value == 0)
		{
			std::cout << "Division by 0.";
			exit(0);
		}
		for (int i = 0; i < n_samples; i++)
		{
			coords[i * n_features + column] /= value;
		}

	}
};

double length_of_column(const vectors& v1, int column)
{
	double distance = 0;
	for (int i = 0; i < v1.n_samples; i++)
	{
		double distance_for_axis = v1.coords[i* v1.n_features + column];
		distance = distance + distance_for_axis * distance_for_axis;
	}
	distance = sqrt(distance);
	return distance;
}

vectors standarise(const vectors& v1)
{
	vectors temp(v1);
	double mean;
	double std_dev;
	for (int i = 0; i < temp.n_features; i++)
	{
		temp.mean(i, &mean);
		temp.substract(mean, i);
		std_dev = length_of_column(temp, i);
		temp.divide(std_dev, i);
	}
	return temp;
}

bool operator==(const vectors& vector1, const vectors& vector2)
{
	if (vector1.n_features != vector2.n_features)
	{
		return false;
	}

	if (vector1.n_samples != vector2.n_samples)
	{
		return false;
	}

	for (int i = 0; i < vector1.n_samples; i++)
	{
		for (int j = 0; j < vector1.n_features; j++)
		{
			if (vector1.coords[i * vector1.n_features + j] != vector2.coords[i * vector1.n_features + j])
			{
				return false;
			}
		}
	}
	return true;
}

vectors std_base(int dimension)
{
	vectors diagonal(dimension);
	for (int i = 0; i < diagonal.n_samples; i++)
	{
		for (int j = 0; j < diagonal.n_features; j++)
		{
			if (i == j)
			{
				diagonal.coords[i * diagonal.n_features + j] = 1.0;
			}
			else
			{
				diagonal.coords[i * diagonal.n_features + j] = 0.0;
			}
		}
	}
	return diagonal;
}

void operator<<(std::ostream& out, const vectors& some_vector)
{
	for (int i = 0; i < some_vector.n_samples; i++)
	{
		for (int j = 0; j < some_vector.n_features; j++)
		{
			out << some_vector.coords[i * some_vector.n_features + j] << ' ';
		}
		std::cout << std::endl;
	}
}

double Euclidean_distance(const vectors& some_vector1, const vectors& some_vector2, int row1, int row2)
{
	double distance = 0;
	for (int i = 0; i < some_vector1.n_features; i++)
	{
		double distance_for_axis = some_vector1.coords[row1 * some_vector1.n_features + i] - some_vector2.coords[row2 * some_vector2.n_features + i];
		distance = distance + distance_for_axis * distance_for_axis;
	}
	distance = sqrt(distance);
	return distance;
}

double cityblock_distance(const vectors& some_vector1, const vectors& some_vector2, int row1, int row2)
{
	double distance = 0;
	for (int i = 0; i < some_vector1.n_features; i++)
	{
		double distance_for_axis = some_vector1.coords[row1 * some_vector1.n_features + i] - some_vector2.coords[row2 * some_vector2.n_features + i];
		if (distance_for_axis < 0)
		{
			distance_for_axis = 0 - distance_for_axis;
		}
		distance = distance + distance_for_axis;
	}
	return distance;
}

double distance(const vectors& some_vector1, const vectors& some_vector2, int row1, int row2, std::string metric = "Euclidean")
{
	if (some_vector1.n_features != some_vector2.n_features)
	{
		std::cout << "The vector dimensionality is inequal.";
		exit(1);
	}
	double distance = 0;
	if (metric == "Euclidean")
	{
		distance = Euclidean_distance(some_vector1, some_vector2, row1, row2);
	}
	//else if (metric == "correlation")
	/*{

	}*/
	else if (metric == "cityblock")
	{
		distance = cityblock_distance(some_vector1, some_vector2, row1, row2);
	}
	return distance;
}
