# include <iostream>
#include<fstream>
# include <cstdlib>
# include <string>
#include <math.h>
#pragma once

enum class dist_ { Euclidean, correlation, cityblock };
// this enum holds available distance metrics

class vectors
// this class holds matrices with the data and enables operations on them
{
public:
	int n_samples = 0; // number of rows
	int n_features = 0; // number of columns
	double* coords; // pointer to memory holding the values within matrix

	vectors() = default;
	// default constructor. Creates NULL pointer

	vectors(int samples, int features, double* data)
	// constructor creating the matrix of given dimensions:
	// samples - number of rows
	// features - number of columns
	// data - a deep copy of a given array is created. The memory used by this parameter should be freed later on, this constructor doesn't free it 
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
	// constructor allocating the memory of the given size (samples*features) and setting the dimensions.
	{
		n_features = features;
		n_samples = samples;
		coords = (double*)malloc(n_features * n_samples* sizeof(double));
	}

	vectors(int square_dim)
	// convenience constructor for square matrices
	{
		n_features = square_dim;
		n_samples = square_dim;
		coords = (double*)malloc(size_t(n_samples * n_features) * sizeof(double));
	}

	vectors(const vectors& v1)
	// copy constructor. Creates a deep copy of a given object - doesn't only copy a pointer coordinates but creates a new one and copies the values in the array
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
	// constructor for loading the vectors from given stream. 
	// The data file should contain only numbers, no letters nor comas. The values should be separated by space.
	// Remember to end the file, as well as each row, with space.
	// For optimisation's sake it doesn't check the correctness of input - errors may occur.
	{
		get_dimensions(datafile);
		coords = (double*)malloc(n_samples*n_features*sizeof(double));
		datafile.clear();
		datafile.seekg(0, std::ios::beg);
		load_data(datafile);
	}

	~vectors()
	// destructor, frees the memory
	{
		free(coords);
	}

	vectors& operator=(const vectors& v)
	// Assignment operator for the calculations and swap needs. Changes the values of dimensions of the current vector to the same as the given one.
	// If the memory was previously unallocated or was of a different size than expected, allocates or reallocates it.
	// Assigns the values of coordinates, not the pointer
	{
		if (n_features != v.n_features || n_samples != v.n_samples)
		{
			n_features = v.n_features;
			n_samples = v.n_samples;
			if (this != NULL)
			{
				coords = (double*)realloc(coords, (n_features * n_samples) * sizeof(double));
			}
			else
			{
				coords = (double*)malloc(n_features * n_samples * sizeof(double));
			}
		}
		for (int i = 0; i < n_samples; i++)
		{
			for (int j = 0; j < n_features; j++)
			{
				coords[i * n_features + j] = v.coords[i * n_features + j];
			}
		}
		return *this;
	}

	void shape_show()
	// method printing the dimensions of the vector
	{
		std::cout << "("<<n_samples << "," << n_features<<")" << std::endl;
	}

	double sum()
	// method calculating the sum of all elements of the array
	{
		double sum = 0;
		for (int i = 0; i < n_features*n_samples; i++)
		{
			sum += coords[i];
		}
		return sum;
	}

	double sum_of_column(int column)
	// calculates the sum of elements in the column of the given index
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

	double sum_of_row(int row) const
	// calculates the sum of elements in the row of the given index
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

	double mean_of_row(int row) const
	// calculates the mean of elements in the row of the given index
	{
		double s = sum_of_row(row);
		return s / n_features;
	}

	double mean_of_column(int column)
	// calculates the mean of elements in the column of the given index
	{
		double s = sum_of_column(column);
		return s / n_samples;
	}

	void change_size(int samples, int features)
	// changes dimensions of the object to given values and reallocates the memory
	{
		n_samples = samples;
		n_features = features;
		coords = (double*)realloc(coords, n_samples * n_features * sizeof(double));
	}

	void substract(double value)
	// method substracting given value from all the elements of the array
	{
		for (int i = 0; i < n_features * n_samples; i++)
		{
			coords[i] -= value;
		}
	}

	void substract(double value, int column)
	// method substracting given value from all the elements of the column specified by given index
	{
		for (int i = 0; i < n_samples; i++)
		{
			coords[i * n_features + column] -= value;
		}
	}

	void substract(double* v, int col)
	// method substracting given vector from the column specified by given index
	// the length of the vector shouldn't be smaller than the length of the column; if it's longer, the additional coords are skipped
	{
		for (int i = 0; i < n_samples; i++)
		{
			coords[i * n_features + col] -= v[i];
		}
	}
	
	double* substract(double value, int row) const
	// method calculating vector specified by the row index decreased by the given value
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
	// method dividing all elements of the column specified by the given index by given value
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
	// method dividing all elements of the array by given value
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
	// method returning the transpose matrix of this one
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

	void leave_n_cols(int col)
	// method removing the columns that are above given number of desired number of features
	{
		vectors temp(n_samples, col);
		for (int i = 0; i < temp.n_samples; i++)
		{
			for (int j = 0; j < temp.n_features; j++)
			{
				temp.coords[i * temp.n_features + j] = coords[i * n_features + j];
			}
		}
		*this = temp;
	}
private:
		void get_dimensions(std::ifstream& datafile)
			// Method counting number of lines and columns in a given file and setting the dimensions of the object accordingly.
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
			// method for loading the data from the given file into the object. The dimensions of the vectors should be specified before the usage.
		{
			for (int i = 0; i < n_samples; i++)
			{
				for (int j = 0; j < n_features; j++)
				{
					datafile >> coords[i * n_features + j];
				}
			}
		}
};

vectors indices(int l)
// function returning 1 sample x l features vector filled with indices
{
	vectors ind(1, l);
	for (int i = 0; i < ind.n_features; i++)
	{
		ind.coords[i] = i;
	}
	return ind;
}

double length_of_column(const vectors& v1, int column)
// this function calculates length of column of v1 under given index
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
// this function calculates length of row of v1 under given index
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
// this function calculates length of given array on specified number of dimensions
// dimension shouldn't be bigger than number of allocated doubles 
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

void normalise(vectors* v, int col)
// this function normalises the given column to unit vector
{
	double n = length_of_column(*v, col);
	v->divide(n, col);
}

void normalise(vectors* v)
// this function normalises whole array
{
	for (int i = 0; i < v->n_features; i++)
	{
		normalise(v, i);
	}
}

vectors center(const vectors& v1)
// this function centres the whole array by substracting from each column its mean
{
	vectors temp(v1);
	double mean;
	for (int i = 0; i < temp.n_features; i++)
	{
		mean = temp.mean_of_column(i);
		temp.substract(mean, i);
	}
	return temp;
}

vectors standardise(const vectors& v1)
// this function takes vectors object and returns its standardised version
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
// this function returns vectors that form standard base for given dimension - diagonal matrix with ones on the diagonal entries
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

bool operator==(const vectors& v1, const vectors& v2)
// it's used to compare two vectors - their dimensions and the fillings of arrays
{
	if (v1.n_features != v2.n_features)
	{
		return false;
	}

	if (v1.n_samples != v2.n_samples)
	{
		return false;
	}

	for (int i = 0; i < v1.n_samples; i++)
	{
		for (int j = 0; j < v1.n_features; j++)
		{
			if (v1.coords[i * v1.n_features + j] != v2.coords[i * v1.n_features + j])
			{
				return false;
			}
		}
	}
	return true;
}

void operator<<(std::ostream& out, const vectors& some_vector)
// this operator is for writing chosen vectors in chosen stream, element by element, with maintaining the dimensions
{
	for (int i = 0; i < some_vector.n_samples; i++)
	{
		for (int j = 0; j < some_vector.n_features; j++)
		{
			out << some_vector.coords[i * some_vector.n_features + j] << ' ';
		}
		out << std::endl;
	}
}

double* operator*(const vectors& A, double* x)
// this operator performs multiplication of single vector x by vectors A
// x should not have less dimensions than row of A, if it's longer, additional features are skipped
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
// this operator performs multiplication of vectors
// the order matters
// number of features of first one should be equal to number of samples of second one
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
// this function returns dot product of two 1D given arrays 
// lengths of both of them should be equal to given dimension
{
	double prod = 0;
	for (int i = 0; i < dimension; i++)
	{
		prod += d1[i] * d2[i];
	}
	return prod;
}

double col_product(vectors x, int col1, int col2)
// this function calculates dot product of 2 columns specified by given indices of vectors x
{
	double prod = 0;
	for (int i = 0; i < x.n_samples; i++)
	{
		prod += x.coords[i * x.n_features + col1] * x.coords[i * x.n_features + col2];
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

double Euclidean_distance(const vectors& v1, const vectors& v2, int row1, int row2)
{
	double distance = 0;
	for (int i = 0; i < v1.n_features; i++)
	{
		double distance_for_axis = v1.coords[row1 * v1.n_features + i] - v2.coords[row2 * v2.n_features + i];
		distance = distance + distance_for_axis * distance_for_axis;
	}
	distance = sqrt(distance);
	return distance;
}

double cityblock_distance(const vectors& v1, const vectors& v2, int row1, int row2)
{
	double distance = 0;
	for (int i = 0; i < v1.n_features; i++)
	{
		double distance_for_axis = v1.coords[row1 * v1.n_features + i] - v2.coords[row2 * v2.n_features + i];
		if (distance_for_axis < 0)
		{
			distance_for_axis = 0 - distance_for_axis;
		}
		distance = distance + distance_for_axis;
	}
	return distance;
}

double distance(const vectors& v1, const vectors& v2, int row1, int row2, dist_ metric = dist_::Euclidean)
{
	if (v1.n_features != v2.n_features)
	{
		std::cout << "The vector dimensionality is inequal.";
		exit(1);
	}
	double distance = 0;
	switch (metric)
	{
	case dist_::Euclidean:
		distance = Euclidean_distance(v1, v2, row1, row2);
		break;
	case dist_::correlation:
		distance = correlation_distance(v1, v2, row1, row2);
		break;
	case dist_::cityblock:
		distance = cityblock_distance(v1, v2, row1, row2);
		break;
	default:
		std::cout << "Unknown distance metric.";
		exit(1);
	}
	return distance;
}

double min_distance(const vectors& v1, const vectors& v2, int row1, dist_ metric = dist_::Euclidean)
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

double min_distance(const vectors& v1, const vectors& v2, int row1, int index, dist_ metric = dist_::Euclidean)
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

int argmin_distance(const vectors& v1, const vectors& v2, int row1, dist_ metric = dist_::Euclidean)
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

double abs(double a, double b)
{
	if (a - b > 0)
		return a - b;
	return b - a;
}

bool are_same(const vectors& v1, const vectors& v2, double tol = 1e-4)
{
	if (v1.n_features != v2.n_features || v1.n_samples != v2.n_samples)
	{
		return false;
	}
	double cur;
	for (int i = 0; i < v1.n_samples; i++)
	{
		for (int j = 0; j < v1.n_features; j++)
		{
			cur = abs(v1.coords[i*v1.n_features+j], v2.coords[i * v2.n_features + j]);
			if (cur > tol)
			{
				return false;
			}
		}
	}
	return true;
}

void orthogonalise(vectors* x, int col1, int col2)
// col1 is the vector we project onto
{
	double* proj = (double*)malloc(sizeof(double) * x->n_samples);
	double scalar = col_product(*x, col1, col2)/ col_product(*x, col1, col1);
	for (int i = 0; i < x->n_samples; i++)
	{
		proj[i] = scalar* x->coords[i * x->n_features + col1];
	}
	x->substract(proj, col2);
	free(proj);
}

vectors Gram_Schmidt(vectors x)
// we assume the vectors are in columns, not rows, while using this function - check your matrix before orthonormalisation
// using MGS for numerical stability
{
	vectors temp(x);
	normalise(&temp, 0);
	for (int i = 1; i < temp.n_features; i++)
	{
		//another loop - substracting projections
		for (int j = 0; j < i; j++)
		{
			orthogonalise(&temp, j, i);
		}
		normalise(&temp, i);
	}
	return temp;
}
