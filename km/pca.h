#pragma once
# include "sorting.h"

vectors covariance_matrix(vectors data)
// returns approximate covariance matrix of data
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

void QR_algorithm(vectors x, vectors* eigenvec, vectors* eigenvals, double tol, int iter)
// finds eigenvectors and eigenvalues of x approximately to tol, iterates at most iter times
{
	vectors Q(x.n_samples, x.n_features), R(x.n_samples, x.n_features);
	vectors A(x);
	vectors U = std_base(x.n_features);
	vectors prev;
	for (int k = 0; k < iter; k++)
	{
		prev = A;
		Q = Gram_Schmidt(A);
		R = (Q.transpose()) * A;
		A = R * Q;
		U = x * U;
		U = Gram_Schmidt(U);
		if (are_same(prev, A, tol))
		{
			break;
		}
	}
	*eigenvec = U; 
	for (int i = 0; i < eigenvals->n_features; i++)
	{
		eigenvals->coords[i] = A.coords[A.n_features * i + i];
	}
}

class PCA 
// Performs Principal Component Analysis
// usage of standardised vectors is recommended
{
public:
	int reduced_dims; // number of features data should have left after the transformation 
	std::string reduce; // if not "YES", keeps all features
	vectors eigenvectors; // eigenvectors of the covariance matrix of data used to fit
	vectors eigenvalues; // eigenvalues of the covariance matrix of data used to fit
	double tol; // tolerance for singular values of coords of eigenvectors
	int n_iter = 1000; // number of iterations for the QR method 

	PCA(std::string red = "YES", int dims = 2, double tolerance = 1e-6) :
	// constructor
		reduced_dims{ dims },
		reduce{ red },
		eigenvectors(1, 1),
		eigenvalues(1, 1),
		tol{ tolerance }
	{}

	~PCA() {}

	PCA(const PCA& t) :
	// copy constructor; creates a deep copy
		reduced_dims{ t.reduced_dims },
		reduce{ t.reduce },
		eigenvectors(t.eigenvectors),
		eigenvalues(t.eigenvalues),
		tol{ t.tol }
	{}

	void fit(vectors data)
	// fit the model with data
	{
		vectors cov = center(data);
		cov = covariance_matrix(cov);
		eigenvectors.change_size(cov.n_samples, cov.n_features);
		eigenvalues.change_size(1, cov.n_samples);
		QR_algorithm(cov, &eigenvectors, &eigenvalues, tol, n_iter);
		sort_two(&eigenvalues, &eigenvectors);
		if (reduce == "YES")
		{
			eigenvectors.leave_n_cols(reduced_dims);
			eigenvalues.leave_n_cols(reduced_dims);
		}
	}

	vectors transform(vectors data)
	//apply dimensionality reduction to data
	//data is projected on the first principal components previously extracted from a training set
	{
		vectors v = center(data);
		v = v * eigenvectors;
		return v;
	}

	vectors fit_transform(vectors data)
	// convenience method
	// equivalent to fitting model with data and applying dimensionality reduction on it.
	{
		fit(data);
		return transform(data);
	}
};
