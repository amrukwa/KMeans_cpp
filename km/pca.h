#pragma once
# include "Vectors.h"

vectors covariance_matrix(vectors data)
{
	vectors cov(data);
	double mean;
	for (int i = 0; i < cov.n_features; i++)
	{
		mean = cov.mean_of_column(i);
		cov.substract(mean, i);
	}
	vectors c = (cov.transpose()) * cov;
	return c;
}

class PCA {
public:
	int reduced_dims;
	std::string reduce;
	vectors eigenvectors;
	vectors eigenvalues;

	PCA(std::string red = "YES", int dims = 2) :
		reduced_dims{ dims },
		reduce{red},
		eigenvectors(1, 1),
		eigenvalues(1,1)
	{}

	~PCA() {}

	PCA(const PCA& t) :
		reduced_dims{t.reduced_dims},
		reduce{t.reduce},
		eigenvectors(t.eigenvectors),
		eigenvalues(t.eigenvalues)
	{}

	void fit(vectors data)
	{
		vectors cov = covariance_matrix(data);
	}

	vectors transform(vectors data)
	{
		vectors v;
		return v;
	}

	vectors fit_transform(vectors data)
	{
		fit(data);
		return transform(data);
	}
};
