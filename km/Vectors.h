# include <iostream>
#include<fstream>
# include <cstdlib>
#pragma once

void get_dimensions(int* n_features, int* n_samples, std::ifstream& datafile);

void check_if_data(char first, char second, int x, int y);

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
		check_if_data(b, c);
	}

	*n_features = x / y;
	*n_samples = y;
	datafile.seekg(0);
}


void check_if_data(char first, char second)
{
	if (first == ' ' || first == '\n' || first == '.')
	{
		if (second == ' ' || second == '\n' || second == '.')
		{
			printf("Check whitespace characters and dots.\n");
			exit(1);
		}
	}
	else if (first == ',')
	{
		printf("Change commas into dots\n");
		exit(1);
	}
	else if (second == EOF)
	{
		if (first != '\n')
		{
			printf("No newline at the end of file.");
			exit(1);
		}
	}
}