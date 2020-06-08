#pragma once
# include "sorting.h"

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

void QR_algorithm(vectors x, vectors* eigenvec, vectors* eigenvals, double tol, int iter)
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


class PCA {
public:
	int reduced_dims;
	std::string reduce;
	vectors eigenvectors;
	vectors eigenvalues;
	double tol;
	int n_iter = 1000;

	PCA(std::string red = "YES", int dims = 2, double tolerance = 1e-6) :
		reduced_dims{ dims },
		reduce{ red },
		eigenvectors(1, 1),
		eigenvalues(1, 1),
		tol{ tolerance }
	{}

	~PCA() {}

	PCA(const PCA& t) :
		reduced_dims{ t.reduced_dims },
		reduce{ t.reduce },
		eigenvectors(t.eigenvectors),
		eigenvalues(t.eigenvalues),
		tol{ t.tol }
	{}

	void fit(vectors data)
	{
		vectors cov = center(data);
		cov = covariance_matrix(cov);
		eigenvectors.change_size(cov.n_samples, cov.n_features);
		eigenvalues.change_size(1, cov.n_samples);
		QR_algorithm(cov, &eigenvectors, &eigenvalues, tol, n_iter);
		// sort them
		// decide how much to leave
	}

	vectors transform(vectors data)
	{
		vectors v = center(data);
		v = v * eigenvectors;
		return v;
	}

	vectors fit_transform(vectors data)
	{
		fit(data);
		return transform(data);
	}
};
