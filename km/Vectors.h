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
		coords = (double*)malloc(sizeof(double) * n_features * n_samples);
		for (int i = 0; i < n_samples; i++)
		{
			for (int j = 0; j < n_features; j++)
			{
				coords[i*n_features+j] = data[i * n_features + j];
			}
		}
	}

	vectors(int samples, int features)
	{
		n_features = features;
		n_samples = samples;
		coords = (double*)malloc(n_features * n_samples* sizeof(double));
	}

	vectors(int square_dim)
	{
		n_features = square_dim;
		n_samples = square_dim;
		coords = (double*)malloc(size_t(n_samples * n_features) * sizeof(double));
	}

	vectors(const vectors& v1)
	{
		n_samples = v1.n_samples;
		n_features = v1.n_features;
		coords = (double*)malloc(sizeof(double) * n_features * n_samples);
		for (int i = 0; i < n_samples; i++)
		{
			for (int j = 0; j < n_features; j++)
			{
				coords[i * n_features + j] = v1.coords[i * n_features + j];
			}
		}
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
	{
		free(coords);
	}

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

	double sum()
	{
		double sum = 0;
		for (int i = 0; i < n_features*n_samples; i++)
		{
			sum += coords[i];
		}
		return sum;
	}

	double sum_of_column(int column)
	{
		if (column >= n_features)
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

	double sum_of_row(int row)
	{
		if (row >= n_samples)
		{
			std::cout << "Invalid index.";
			exit(1);
		}
		double sum = 0;
		for (int i = 0; i < n_features; i++)
		{
			sum += coords[row * n_features + i];
		}
		return sum;
	}

	double sum_of_row(int row) const
	{
		if (row >= n_samples)
		{
			std::cout << "Invalid index.";
			exit(1);
		}
		double sum = 0;
		for (int i = 0; i < n_features; i++)
		{
			sum += coords[row * n_features + i];
		}
		return sum;
	}

	double mean_of_row(int row)
	{
		double s = sum_of_column(row);
		return s / n_features;
	}

	double mean_of_row(int row) const
	{
		double s = sum_of_row(row);
		return s / n_features;
	}

	double mean_of_column(int column) 
	{
		double s = sum_of_column(column);
		return s / n_samples;
	}

	void change_size(int samples, int features)
	{
		n_samples = samples;
		n_features = features;
		coords = (double*)realloc(coords, n_samples * n_features * sizeof(double));
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
	
	double* substract(double value, int row) const
	{
		static double* ptr;
		ptr = (double*)malloc(n_features* sizeof(double));
		for (int i = 0; i < n_features; i++)
		{
			ptr[i] = coords[row * n_features + i] - value;
		}

		return ptr;
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

	void divide(double value)
	{
		if (value == 0)
		{
			std::cout << "Division by 0.";
			exit(0);
		}
		for (int i = 0; i < n_features * n_samples; i++)
		{
			coords[i] /= value;
		}
	}

	vectors transpose()
	{
		double* data;
		data = (double*)malloc(sizeof(double) * size_t(double(n_features) * n_samples));
		for (int i = 0; i < n_samples; i++)
		{
			for (int j = 0; j < n_features; j++)
			{
				data[n_samples * j + i] = coords[n_features * i + j];
			}
		}
		vectors temp(n_features, n_samples, data);
		return temp;
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

double length_of_row(const vectors& v1, int row)
{
	double distance = 0;
	for (int i = 0; i < v1.n_features; i++)
	{
		double distance_for_axis = v1.coords[row * v1.n_features + i];
		distance = distance + distance_for_axis * distance_for_axis;
	}
	distance = sqrt(distance);
	return distance;
}

double length_of_row(double* v, int dimension)
{
	double distance = 0;
	for (int i = 0; i < dimension; i++)
	{
		double distance_for_axis = v[i];
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
		mean  = temp.mean_of_column(i);
		temp.substract(mean, i);
		std_dev = length_of_column(temp, i)/sqrt(temp.n_samples);
		temp.divide(std_dev, i);
	}
	return temp;
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

double* operator*(const vectors& A, double* x)
{
	static double* ptr;
	ptr = (double*)calloc(A.n_samples, sizeof(double));
	for (int i = 0; i < A.n_samples; i++)
	{
		for (int j = 0; j < A.n_features; j++)
		{
			ptr[i] += A.coords[i * A.n_features + j] * x[j];
		}
	}
	return ptr;
}

vectors operator*(const vectors& A, const vectors& B)
{
	double* c;
	c = (double*)calloc(size_t(B.n_features * A.n_samples), sizeof(double*));
	for (int i = 0; i < A.n_samples; i++)
	{
		for (int j = 0; j < B.n_features; j++)
		{
			for (int k = 0; k < A.n_features; k++)
			{
				c[B.n_features * i + j] += A.coords[i * A.n_features + k]* B.coords[k * B.n_features + j];
			}
		}
	}
	vectors C(A.n_samples, B.n_features, c);
	return C;
}

double row_product(double* d1, double* d2, int dimension)
{
	double prod = 0;
	for (int i = 0; i < dimension; i++)
	{
		prod += d1[i] * d2[i];
	}
	return prod;
}

double correlation_distance(const vectors& v1, const vectors& v2, int row1, int row2)
{
	double distance = 0;
	double* u;
	double* v;
	double denom;
	double mean_u = v1.mean_of_row(row1);
	double mean_v = v2.mean_of_row(row2);
	u = v1.substract(mean_u, row1);
	v = v2.substract(mean_v, row2);
	distance = row_product(u, v, v1.n_features);
	denom = length_of_row(u, v1.n_features)* length_of_row(v, v2.n_features);
	distance = 1 - distance/denom;
	free(u);
	free(v);
	return distance;
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
	else if (metric == "cityblock")
	{
		distance = cityblock_distance(some_vector1, some_vector2, row1, row2);
	}
	else if (metric == "correlation")
	{
		distance = correlation_distance(some_vector1, some_vector2, row1, row2);
	}
	else
	{
		std::cout << "Unknown distance metric.";
		exit(1);
	}
	return distance;
}

double min_distance(const vectors& v1, const vectors& v2, int row1, std::string metric = "Euclidean")
{
	//calculates the distance to the closest member of v2 from row1 of v1 
	double min_dist = distance(v1, v2, row1, 0, metric);
	double cur_dist;
	for (int i = 1; i < v2.n_samples; i++)
	{
		cur_dist = distance(v1, v2, row1, i, metric);
		if (cur_dist < min_dist)
		{
			min_dist = cur_dist;
		}
	}
	return min_dist;
}

double min_distance(const vectors& v1, const vectors& v2, int row1, int index, std::string metric = "Euclidean")
{
	//calculates the distance to the closest member of v2 (up to index, exclusively) from row1 of v1 
	double min_dist = distance(v1, v2, row1, 0, metric);
	double cur_dist;
	for (int i = 1; i < index; i++)
	{
		cur_dist = distance(v1, v2, row1, i, metric);
		if (cur_dist < min_dist)
		{
			min_dist = cur_dist;
		}
	}
	return min_dist;
}

int argmin_distance(const vectors& v1, const vectors& v2, int row1, std::string metric = "Euclidean")
{
	//calculates the index of the closest member of v2 from row1 of v1 
	double min_dist = distance(v1, v2, row1, 0, metric);
	double cur_dist;
	int index = 0;
	for (int i = 1; i < v2.n_samples; i++)
	{
		cur_dist = distance(v1, v2, row1, i, metric);
		if (cur_dist < min_dist)
		{
			min_dist = cur_dist;
			index = i;
		}
	}
	return index;
}
