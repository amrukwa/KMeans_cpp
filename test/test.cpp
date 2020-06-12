#include "pch.h"
#include "CppUnitTest.h"
#include "dunn.h"
# include "pca.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace test_vectors
{
	TEST_CLASS(test_vectors)
	{
	public:
		void check_matrix_filling(vectors* some_vector, double* data, int i, int j)
		{
			Assert::AreEqual(some_vector->coords[i * some_vector->n_features + j], data[i * some_vector->n_features + j]);
		}

		void check_loading_data(vectors some_vector, int i, int j)
		{
			Assert::AreEqual(some_vector.coords[i * some_vector.n_features + j], double(i * some_vector.n_features + j));
		}

		void check_for_ones(vectors some_vector, int i, int j)
		{
			if (i == j)
			{
				Assert::AreEqual(some_vector.coords[i * some_vector.n_features + j], 1.0);
			}
			else
			{
				Assert::AreEqual(some_vector.coords[i * some_vector.n_features + j], 0.0);
			}
		}

		void check_mean_of_column(vectors some_vector, int i)
		{
			double mean = 0;
			mean = some_vector.mean_of_column(i);
			Assert::AreEqual(mean, 2.0 * (double(i)+1.0));
		}

		TEST_METHOD(Test_vector_is_copied)
		{
			double* data = (double*)malloc(20 * sizeof(double));
			data[0] = 0.0;
			vectors some_vector(1, 1, data);
			vectors other_vector(some_vector);
			Assert::AreEqual(0.0, other_vector.coords[0]);
			Assert::AreEqual(some_vector.n_samples, other_vector.n_samples);
			Assert::AreEqual(some_vector.n_features, other_vector.n_features);
			free(data);
		}

		TEST_METHOD(Test_dimensions_are_assigned)
		{
			double* data = (double*)malloc(20*sizeof(double));
			vectors some_vector(2, 10, data);
			Assert::AreEqual(some_vector.n_features, 10);
			Assert::AreEqual(some_vector.n_samples, 2);
			free(data);
		}

		TEST_METHOD(Test_matrix_is_filled)
		{
			double* data = (double*)malloc(6 * sizeof(double));
			for (int i = 0; i < 2; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					data[i * 3 + j] = (i * 3) + j;
				}
			}
			vectors some_vector(2, 3, data);
			for (int i = 0; i < 2; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					check_matrix_filling(&some_vector, data, i, j);
				}
			}
			free(data);
		}

		TEST_METHOD(Test_diagonal_matrix)
		{
			vectors diagonal = std_base(2);
			Assert::AreEqual(diagonal.n_samples, 2);
			Assert::AreEqual(diagonal.n_features, 2);
			for (int i = 0; i < diagonal.n_samples; i++)
			{
				for (int j = 0; j < diagonal.n_features; j++)
				{
					check_for_ones(diagonal, i, j);
				}
			}
		}

		TEST_METHOD(Test_dimensions_equality)
		{
			vectors d2 = std_base(2);
			vectors d3 = std_base(3);
			Assert::AreEqual(int(d2 == d3), 0);
			Assert::AreEqual(int(d2 == d2), 1);
		}

		TEST_METHOD(Test_data_equality)
		{
			double* data;
			data = (double*)malloc(sizeof(double) * 4);
			for (int i = 0; i < 4; i++)
			{
				data[i] = double(i);
			}
			vectors d2 = std_base(2);
			vectors d1(2, 2, data);
			Assert::AreEqual(int(d2 == d1), 0);
			free(data);
		}

		TEST_METHOD(Test_mean_of_column)
		{
			double* data;
			data = (double*)malloc(sizeof(double) * 4);
			for (int i = 0; i < 4; i++)
			{
				data[i] = double(2*i);
			}
			vectors d1(2, 2, data);
			for (int i = 0; i < d1.n_features; i++)
			{
				check_mean_of_column(d1, i);
			}
			free(data);
		}

		TEST_METHOD(Test_substracting_value)
		{
			double* data1;
			double* data2;
			data1 = (double*)malloc(sizeof(double) * 4);
			data2 = (double*)malloc(sizeof(double) * 4);
			for (int i = 0; i < 4; i++)
			{
				data1[i] = double(i);
				data2[i] = double(i-1);
			}
			vectors d1(2, 2, data1);
			d1.substract(1);
			for (int i = 0; i < 2; i++)
			{
				for (int j = 0; j < 2; j++)
				{
					check_matrix_filling(&d1, data2, i, j);
				}
			}
			free(data1);
			free(data2);
		}

		TEST_METHOD(Test_substracting_from_col)
		{
			double* data1;
			double* data2;
			data1 = (double*)malloc(sizeof(double) * 4);
			data2 = (double*)malloc(sizeof(double) * 4);
			for (int i = 0; i < 4; i++)
			{
				data1[i] = double(i);
			}
			data2[0] = 0;
			data2[1] = 0;
			data2[2] = 2;
			data2[3] = 2;
			vectors d1(2, 2, data1);
			d1.substract(1, 1);
			for (int i = 0; i < 2; i++)
			{
				for (int j = 0; j < 2; j++)
				{
					check_matrix_filling(&d1, data2, i, j);
				}
			}
			free(data1);
			free(data2);
		}

		TEST_METHOD(Test_col_division)
		{
			double* data1;
			double* data2;
			data1 = (double*)malloc(sizeof(double) * 4);
			data2 = (double*)malloc(sizeof(double) * 4);
			for (int i = 0; i < 4; i++)
			{
				data1[i] = double(i);
			}
			data2[0] = 0;
			data2[1] = 1.0/3;
			data2[2] = 1.0;
			data2[3] = 1.0;
			vectors d1(2, 2, data1);
			d1.divide(2, 0);
			d1.divide(3, 1);
			for (int i = 0; i < d1.n_samples; i++)
			{
				for (int j = 0; j < d1.n_features; j++)
				{
					check_matrix_filling(&d1, data2, i, j);
				}
			}
			free(data1);
			free(data2);
		}

		TEST_METHOD(Test_division)
		{
			double* data1;
			double* data2;
			data1 = (double*)malloc(sizeof(double) * 4);
			data2 = (double*)malloc(sizeof(double) * 4);
			for (int i = 0; i < 4; i++)
			{
				data1[i] = double(i);
				data2[i] = double(i) / 2;
			}
			vectors d1(2, 2, data1);
			d1.divide(2);
			for (int i = 0; i < d1.n_samples; i++)
			{
				for (int j = 0; j < d1.n_features; j++)
				{
					check_matrix_filling(&d1, data2, i, j);
				}
			}
			free(data1);
			free(data2);
		}

		TEST_METHOD(Test_column_length)
		{
			vectors d1 = std_base(2);
			d1.substract(1, 1);
			Assert::AreEqual(length_of_column(d1, 0), 1.0);
			Assert::AreEqual(length_of_column(d1, 1), 1.0);
		}

		TEST_METHOD(Test_transpose)
		{
			double* d1;
			d1 = (double*)malloc(sizeof(double) * 6);
			double* d2;
			d2 = (double*)malloc(sizeof(double) * 6);
			d1[0] = -1;
			d1[1] = 3;
			d1[2] = 4;
			d1[3] = -2;
			d1[4] = 5;
			d1[5] = 0;
			d2[0] = -1;
			d2[1] = 4;
			d2[2] = 5;
			d2[3] = 3;
			d2[4] = -2;
			d2[5] = 0;
			vectors A(3, 2, d1);
			vectors B = A.transpose();
			Assert::AreEqual(B.n_features, 3);
			Assert::AreEqual(B.n_samples, 2);
			for (int i = 0; i < B.n_samples; i++)
			{
				for (int j = 0; j < B.n_features; j++)
				{
					check_matrix_filling(&B, d2, i, j);
				}
			}
			free(d1);
			free(d2);
		}

		TEST_METHOD(Test_covariance)
		{
			double* d1;
			d1 = (double*)malloc(sizeof(double) * 6);
			double* d2;
			d2 = (double*)malloc(sizeof(double) * 4);
			d1[0] = -1;
			d1[1] = 3;
			d1[2] = 4;
			d1[3] = -2;
			d1[4] = 5;
			d1[5] = 0;
			d2[0] = 42;
			d2[1] = -11;
			d2[2] = -11;
			d2[3] = 13;
			vectors A(3, 2, d1);
			vectors B = (A.transpose())*A;
			Assert::AreEqual(B.n_features, 2);
			Assert::AreEqual(B.n_samples, 2);
			for (int i = 0; i < B.n_samples; i++)
			{
				for (int j = 0; j < B.n_features; j++)
				{
					check_matrix_filling(&B, d2, i, j);
				}
			}
			free(d1);
			free(d2);
		}

		TEST_METHOD(Test_cov)
		{
			double* d1;
			d1 = (double*)malloc(sizeof(double) * 6);
			double* d2;
			d2 = (double*)malloc(sizeof(double) * 4);
			d1[0] = -1;
			d1[1] = 4;
			d1[2] = 2;
			d1[3] = 0;
			d1[4] = 5;
			d1[5] = 2;
			d2[0] = 18;
			d2[1] = -6;
			d2[2] = -6;
			d2[3] = 8;
			vectors A(3, 2, d1);
			vectors B = covariance_matrix(A);
			Assert::AreEqual(B.n_features, 2);
			Assert::AreEqual(B.n_samples, 2);
			for (int i = 0; i < B.n_samples; i++)
			{
				for (int j = 0; j < B.n_features; j++)
				{
					check_matrix_filling(&B, d2, i, j);
				}
			}
			free(d1);
			free(d2);
		}

		TEST_METHOD(Test_substracted_vector)
		{
			double* data1;
			double* data2;
			data1 = (double*)malloc(sizeof(double) * 4);
			data2 = (double*)malloc(sizeof(double) * 2);
			for (int i = 0; i < 4; i++)
			{
				data1[i] = double(i);
			}
			data2[0] = 1;
			data2[1] = 2;
			const vectors d1(2, 2, data1);
			double* ptr = d1.substract(1, 1);
			for (int i = 0; i < 2; i++)
			{
					Assert::AreEqual(ptr[i], data2[i]);
			}
			free(ptr);
			free(data1);
			free(data2);
		}

		TEST_METHOD(Test_length_row)
		{
			double* r;
			r = (double*)malloc(sizeof(double) * 3);
			r[0] = 0;
			r[1] = 3;
			r[2] = 4;
			double prod = length_of_row(r, 3);
			Assert::AreEqual(5.0, prod);
			free(r);
		}

		TEST_METHOD(Test_sum)
		{
			double* data;
			double s = 21.0;
			data = (double*)malloc(sizeof(double) * 6);
			for (int i = 0; i < 6; i++)
			{
				data[i] = double(i) + 1;
			}
			vectors v(2, 3, data);
			double sum = v.sum();
			Assert::AreEqual(s, sum);
		}

		TEST_METHOD(Test_abs)
		{
			double a = 4;
			double b = 7;
			Assert::AreEqual(3.0, abs(a, b));
		}

		TEST_METHOD(Test_tol_equality)
		{
			double* c = (double*)malloc(sizeof(double) * 4);
			double* d = (double*)malloc(sizeof(double) * 4);
			c[0] = 0;
			d[0] = 0.00001;
			c[1] = 1.1;
			d[1] = 1.1;
			c[2] = -2;
			d[2] = -2.00001;
			c[3] = 6.199999;
			d[3] = 6.2;
			vectors vc(2, 2, c);
			vectors vd(2, 2, d);
			Assert::AreEqual(true, are_same(vc, vd));
		}

		TEST_METHOD(Test_deep_asignment)
		{
			double* d = (double*)malloc(sizeof(double) * 4);
			d[0] = 1.0;
			d[1] = 2.0;
			d[2] = 3.0;
			d[3] = 4.0;
			vectors v2(3, 1);
			vectors v1(2, 2, d);
			v2 = v1;
			v1.divide(2.0);
			for (int i = 0; i < 4; i++)
			{
				Assert::AreEqual(2 * v1.coords[i], v2.coords[i]);
			}
			free(d);
		}

		TEST_METHOD(Test_col_product)
		{
			double* d = (double*)malloc(sizeof(double) * 4);
			d[0] = 1;
			d[2] = 2;
			d[1] = 4;
			d[3] = 5;
			double dot = 14.0;
			vectors data(2, 2, d);
			Assert::AreEqual(dot, col_product(data, 0, 1));
			free(d);
		}

		TEST_METHOD(Test_substracting_cols)
		{
			double* d = (double*)malloc(sizeof(double) * 4);
			d[0] = 3.0;
			d[1] = 4.0;
			d[2] = 5.0;
			d[3] = 6.0;
			double* c = (double*)malloc(sizeof(double) * 4);
			vectors check(2, 2, d);
			c[0] = 2.0;
			c[1] = 4.0;
			c[2] = 1.0;
			c[3] = 6.0;
			double* s = (double*)malloc(sizeof(double) * 2);
			s[0] = 1.0;
			s[1] = 4.0;
			check.substract(s, 0);
			for (int i = 0; i < check.n_samples; i++)
			{
				for (int j = 0; j < check.n_features; j++)
				{
					check_matrix_filling(&check, c, i, j);
				}
			}
			free(d);
			free(c);
			free(s);
		}

		TEST_METHOD(Test_orthogonalisation)
		{
			double* d = (double*)malloc(sizeof(double) * 6);
			d[0] = 1.0;
			d[1] = 1.0;
			d[2] = 1.0;
			d[3] = 2.0;
			d[4] = 0.0;
			d[5] = 0.0;
			vectors x(3, 2, d);
			d[1] = -0.5;
			d[3] = 0.5;
			vectors orth(3, 2, d);
			orthogonalise(&x, 0, 1);
			for (int i = 0; i < x.n_samples; i++)
			{
				for (int j = 0; j < x.n_features; j++)
				{
					Assert::AreEqual(orth.coords[i * x.n_features + j], x.coords[i * x.n_features + j], 1e-6);
				}
			}
			free(d);
		}
		
		TEST_METHOD(Test_QR)
		{
			vectors d(2, 2);
			double* evc = (double*)malloc(sizeof(double) * 4);
			double* evl = (double*)malloc(sizeof(double) * 2);
			vectors evec(2, 2);
			vectors evals(1, 2);
			d.coords[0] = 2.0;
			d.coords[1] = 0.0;
			d.coords[2] = 0.0;
			d.coords[3] = -1.0;
			evc[0] = 1.0;
			evc[1] = 0.0;
			evc[2] = 0.0;
			evc[3] = -1.0;
			evl[0] = 2.0;
			evl[1] = -1.0;
			QR_algorithm(d, &evec, &evals, 1e-4, 100);
			for (int i = 0; i < 2; i++)
			{
				Assert::AreEqual(evl[i], evals.coords[i]);
				for (int j = 0; j < 2; j++)
				{
					Assert::AreEqual(evc[i*evec.n_features+j], evec.coords[i * evec.n_features + j]);
				}
			}
			free(evc);
			free(evl);
		}
	};

	TEST_CLASS(test_vector_products)
	{
	public:
		TEST_METHOD(Test_matrix_multiplication)
		{
			double* d1;
			double* d2;
			d2 = (double*)malloc(sizeof(double) * 4);
			d2[0] = -3;
			d2[1] = 2;
			d2[2] = -4;
			d2[3] = 1;
			d1 = (double*)malloc(sizeof(double) * 6);
			d1[0] = -1;
			d1[1] = 3;
			d1[2] = 4;
			d1[3] = -2;
			d1[4] = 5;
			d1[5] = 0;
			vectors A(3, 2, d1);
			vectors B(2, 2, d2);
			vectors C = A * B;
			double* d3;
			d3 = (double*)malloc(sizeof(double) * 6);
			d3[0] = -9;
			d3[1] = 1;
			d3[2] = -4;
			d3[3] = 6;
			d3[4] = -15;
			d3[5] = 10;
			Assert::AreEqual(C.n_features, 2);
			Assert::AreEqual(C.n_samples, 3);
			for (int i = 0; i < C.n_samples; i++)
			{
				for (int j = 0; j < C.n_features; j++)
				{
					Assert::AreEqual(C.coords[i * C.n_features + j], d3[i * C.n_features + j]);
				}
			}
			free(d1);
			free(d2);
			free(d3);
		}

		TEST_METHOD(Test_vector_multiplication)
		{
			double* data;
			data = (double*)malloc(sizeof(double) * 6);
			data[0] = 1;
			data[1] = 1;
			data[2] = 1;
			data[3] = double(3) / 5;
			data[4] = 0;
			data[5] = double(4) / 5;
			vectors v(2, 3, data);
			double dot = row_product(v.coords, v.coords + v.n_features, v.n_features);
			Assert::AreEqual(double(7) / 5, dot);
			free(data);
		}

		TEST_METHOD(Test_linear_transformation)
		{
			double* data;
			data = (double*)malloc(sizeof(double) * 9);
			data[0] = 1;
			data[1] = 2;
			data[2] = 3;
			data[3] = 3;
			data[4] = 2;
			data[5] = 1;
			data[6] = 1;
			data[7] = 2;
			data[8] = 3;
			vectors v(3, 3, data);
			double* d;
			d = (double*)malloc(sizeof(double) * 3);
			d[0] = 4;
			d[1] = 5;
			d[2] = 6;
			double* c;
			c = (double*)malloc(sizeof(double) * 3);
			c[0] = 32;
			c[1] = 28;
			c[2] = 32;
			double* e;
			e = (double*)malloc(sizeof(double) * 3);
			e = v * d;
			Assert::AreEqual(c[0], e[0]);
			Assert::AreEqual(c[1], e[1]);
			Assert::AreEqual(c[2], e[2]);
			free(data);
			free(d);
			free(c);
			free(e);
		}
	};

	TEST_CLASS(test_prep)
	{
	public:
		TEST_METHOD(Test_standardising)
		{
			double* data;
			data = (double*)malloc(sizeof(double) * 6);
			for (int i = 0; i < 6; i++)
			{
				data[i] = double(i) + 1;
			}
			vectors v(2, 3, data);
			double* c;
			c = (double*)malloc(sizeof(double) * 6);
			c[0] = -1;
			c[1] = -1;
			c[2] = -1;
			c[3] = 1;
			c[4] = 1;
			c[5] = 1;
			vectors d = standarise(v);
			for (int i = 0; i < 2; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					Assert::AreEqual(d.coords[i * d.n_features + j], c[i * d.n_features + j], 1e-4);
				}
			}
			free(data);
		}

		TEST_METHOD(Test_normalising)
		{
			double* d = (double*)malloc(sizeof(double) * 9);
			double* c = (double*)malloc(sizeof(double) * 9);
			d[0] = 4.0;
			d[1] = 0.0;
			d[2] = 1.0;
			d[3] = 3.0;
			d[4] = 12.0;
			d[5] = 1.0;
			d[6] = 0.0;
			d[7] = 5.0;
			d[8] = 1.0;
			vectors x(3, 3, d);
			normalise(&x);
			c[0] = 4.0 / 5;
			c[1] = 0.0;
			c[2] = 1.0 / sqrt(3);
			c[3] = 3.0 / 5;
			c[4] = 12.0 / 13;
			c[5] = 1.0 / sqrt(3);
			c[6] = 0.0;
			c[7] = 5.0 / 13;
			c[8] = 1.0 / sqrt(3);
			for (int i = 0; i < x.n_samples; i++)
			{
				for (int j = 0; j < x.n_features; j++)
				{
					Assert::AreEqual(c[i * x.n_features + j], x.coords[i * x.n_features + j], 1e-4);
				}
			}
			free(c);
			free(d);
		}

		TEST_METHOD(Test_centering)
		{
			double* data;
			data = (double*)malloc(sizeof(double) * 6);
			for (int i = 0; i < 6; i++)
			{
				data[i] = double(i) + 1;
			}
			vectors v(2, 3, data);
			double* c;
			c = (double*)malloc(sizeof(double) * 6);
			c[0] = -1.5;
			c[1] = -1.5;
			c[2] = -1.5;
			c[3] = 1.5;
			c[4] = 1.5;
			c[5] = 1.5;
			vectors d = center(v);
			for (int i = 0; i < 2; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					Assert::AreEqual(d.coords[i * d.n_features + j], c[i * d.n_features + j], 1e-4);
				}
			}
			free(data);
		}

		TEST_METHOD(Test_Gramm_Schmidt)
		{
			double* m = (double*)malloc(sizeof(double) * 9);
			double* o = (double*)malloc(sizeof(double) * 9);
			m[0] = 1.0;
			m[1] = 1.0;
			m[2] = 0.0;
			m[3] = 1.0;
			m[4] = 2.0;
			m[5] = 1.0;
			m[6] = 0.0;
			m[7] = 0.0;
			m[8] = 2.0;
			o[0] = sqrt(2) / 2;
			o[1] = -sqrt(2) / 2;
			o[2] = 0.0;
			o[3] = sqrt(2) / 2;
			o[4] = sqrt(2) / 2;
			o[5] = 0.0;
			o[6] = 0.0;
			o[7] = 0.0;
			o[8] = 1.0;
			vectors x(3, 3, m);
			vectors orth = Gram_Schmidt(x);
			for (int i = 0; i < x.n_samples; i++)
			{
				for (int j = 0; j < x.n_features; j++)
				{
					Assert::AreEqual(o[i * x.n_features + j], orth.coords[i * x.n_features + j], 1e-6);
				}
			}
		}
	};

	TEST_CLASS(test_distance)
	{
	public:
		TEST_METHOD(Test_distance)
		{
			double* data;
			double dist;
			data = (double*)malloc(sizeof(double) * 6);
			for (int i = 0; i < 6; i++)
			{
				data[i] = double(i) + 1;
			}
			vectors v(2, 3, data);
			dist = distance(v, v, 0, 1);
			Assert::AreEqual(3 * sqrt(3), dist);
			free(data);
		}

		TEST_METHOD(Test_cityblock_distance)
		{
			double* data;
			double dist;
			data = (double*)malloc(sizeof(double) * 6);
			for (int i = 0; i < 6; i++)
			{
				data[i] = double(i) + 1;
			}
			vectors v(2, 3, data);
			dist = distance(v, v, 0, 1, dist_::cityblock);
			Assert::AreEqual(9.0, dist);
			free(data);
		}

		TEST_METHOD(Test_correlation_distance)
		{
			double* data;
			double dist;
			data = (double*)malloc(sizeof(double) * 6);
			for (int i = 0; i < 6; i++)
			{
				data[i] = double(i) + 1;
			}
			vectors v(2, 3, data);
			dist = distance(v, v, 0, 1, dist_::correlation);
			Assert::AreEqual(2.22044605e-16, dist, 1e-24);
			free(data);
		}

		TEST_METHOD(Test_min_dist)
		{
			double* d1 = (double*)malloc(sizeof(double) * 3);
			double* d2 = (double*)malloc(sizeof(double) * 9);
			d1[0] = 1;
			d1[1] = 1;
			d1[2] = 1;
			d2[0] = 3;
			d2[1] = 5;
			d2[2] = 7;
			d2[3] = 0;
			d2[4] = 0;
			d2[5] = 0;
			d2[6] = 7;
			d2[7] = 8;
			d2[8] = 9;
			vectors v2(3, 3, d2);
			vectors v1(1, 3, d1);
			double dist = min_distance(v1, v2, 0);
			Assert::AreEqual(sqrt(3), dist, 1e-4);
			free(d1);
			free(d2);
		}

		TEST_METHOD(Test_min_dist_ind)
		{
			double* d1 = (double*)malloc(sizeof(double) * 3);
			double* d2 = (double*)malloc(sizeof(double) * 9);
			d1[0] = 1;
			d1[1] = 1;
			d1[2] = 1;
			d2[0] = 3;
			d2[1] = 5;
			d2[2] = 7;
			d2[3] = 7;
			d2[4] = 8;
			d2[5] = 9;
			d2[6] = 0;
			d2[7] = 0;
			d2[8] = 0;
			vectors v2(3, 3, d2);
			vectors v1(1, 3, d1);
			double dist = min_distance(v1, v2, 0, 2);
			Assert::AreEqual(sqrt(56), dist, 1e-4);
			free(d1);
			free(d2);
		}

		TEST_METHOD(Test_argmin_dist)
		{
			double* d1 = (double*)malloc(sizeof(double) * 3);
			double* d2 = (double*)malloc(sizeof(double) * 9);
			d1[0] = 1;
			d1[1] = 1;
			d1[2] = 1;
			d2[0] = 3;
			d2[1] = 5;
			d2[2] = 7;
			d2[3] = 0;
			d2[4] = 0;
			d2[5] = 0;
			d2[6] = 7;
			d2[7] = 8;
			d2[8] = 9;
			vectors v2(3, 3, d2);
			vectors v1(1, 3, d1);
			int ind = argmin_distance(v1, v2, 0);
			Assert::AreEqual(1, ind);
			free(d1);
			free(d2);
		}
	};
}

namespace test_kmeans
{
	TEST_CLASS(test_kmeans)
	{
	public:
		TEST_METHOD(Test_kmeans_const)
		{
			kmeans k(2);
			kmeans cpy(k);
			k.centroids.coords[0] = 1;
			Assert::AreNotEqual(1.0, cpy.centroids.coords[0]);
			Assert::AreEqual(1.0, k.centroids.coords[0]);
		}

		TEST_METHOD(Test_random_kmeans_init)
		{
			double* a = (double*)malloc(sizeof(double) * 3);
			a[0] = 0;
			a[1] = 1;
			a[2] = 2;
			vectors x(3, 1, a);
			vectors c(3, 1);
			initialize(&c, x, init_::random, dist_::Euclidean);
			Assert::IsNotNull(c.coords);
			Assert::AreNotEqual(c.coords[0], c.coords[1]);
			Assert::AreNotEqual(c.coords[1], c.coords[2]);
			Assert::AreNotEqual(c.coords[0], c.coords[2]);
			free(a);
		}

		TEST_METHOD(Test_labeling)
		{
			double* x_d = (double*)malloc(sizeof(double) * 10);
			double* c_d = (double*)malloc(sizeof(double) * 6);
			c_d[0] = -3;
			c_d[1] = -2; // 0
			c_d[2] = -6;
			c_d[3] = 3; // 1
			c_d[4] = 5;
			c_d[5] = 2; // 2

			x_d[0] = -4;
			x_d[1] = 0; // 0
			x_d[2] = -5;
			x_d[3] = 3; // 1
			x_d[4] = 0;
			x_d[5] = 0; // 0
			x_d[6] = 4;
			x_d[7] = 3; // 2
			x_d[8] = 6;
			x_d[9] = 1; // 2
			vectors x(5, 2, x_d);
			vectors c(3, 2, c_d);
			vectors l(6, 1);
			label_points(&l, x, c, dist_::Euclidean);
			Assert::AreEqual(0.0, l.coords[0]);
			Assert::AreEqual(1.0, l.coords[1]);
			Assert::AreEqual(0.0, l.coords[2]);
			Assert::AreEqual(2.0, l.coords[3]);
			Assert::AreEqual(2.0, l.coords[4]);
			free(x_d);
			free(c_d);
		}

		TEST_METHOD(Test_calc_centroids)
		{
			double* x_d = (double*)malloc(sizeof(double) * 10);
			double* c_d = (double*)malloc(sizeof(double) * 6);
			double* c_real = (double*)malloc(sizeof(double) * 6);
			c_d[0] = -3;
			c_d[1] = -2; // 0
			c_d[2] = -6;
			c_d[3] = 3; // 1
			c_d[4] = 5;
			c_d[5] = 2; // 2

			x_d[0] = -4;
			x_d[1] = 0; // 0
			x_d[2] = -5;
			x_d[3] = 3; // 1
			x_d[4] = 0;
			x_d[5] = 0; // 0
			x_d[6] = 4;
			x_d[7] = 3; // 2
			x_d[8] = 6;
			x_d[9] = 1; // 2

			c_real[0] = -2;
			c_real[1] = 0;
			c_real[2] = -5;
			c_real[3] = 3;
			c_real[4] = 5;
			c_real[5] = 2;

			vectors x(5, 2, x_d);
			vectors c(3, 2, c_d);
			vectors l(6, 1);
			label_points(&l, x, c, dist_::Euclidean);
			calculate_centroids(l, x, &c);
			for (int i = 0; i < c.n_samples; i++)
			{
				for (int j = 0; j < c.n_features; j++)
				{
					Assert::AreEqual(c.coords[i * c.n_features + j], c_real[i * c.n_features + j]);
				}
			}
			free(c_d);
			free(c_real);
			free(x_d);
		}

		TEST_METHOD(Test_inertia)
		{
			double* x = (double*)malloc(sizeof(double) * 9);
			double* c = (double*)malloc(sizeof(double) * 6);
			double* l = (double*)malloc(sizeof(double) * 3);
			for (int i = 0; i < 9; i++)
			{
				x[i] = double(i) + 1;
			}
			vectors data(3, 3, x);
			c[0] = 2;
			c[1] = 3;
			c[2] = 4;
			c[3] = 10;
			c[4] = 10;
			c[5] = 10;
			vectors centres(2, 3, c);
			l[0] = 0;
			l[1] = 0;
			l[2] = 1;
			vectors labels(1, 3, l);
			double inertia = calculate_inertia(labels, data, centres, dist_::Euclidean);
			Assert::AreEqual(29.0, inertia);
			free(x);
			free(c);
			free(l);
		}
	};

}

namespace test_sorting
{
	TEST_CLASS(test_sorting)
	{
	public:
		TEST_METHOD(Test_ind_v)
		{
			vectors basic(2, 2);
			vectors i = indices(2);
			for (int j = 0; j < i.n_features; j++)
			{
				Assert::AreEqual(j, int(i.coords[j]));
			}
		}

		TEST_METHOD(Test_sort_desc)
		{
			double* data = (double*)malloc(sizeof(double) * 2);
			data[0] = 5;
			data[1] = 10;
			vectors c(1, 2, data);
			std::sort(c.coords, c.coords + (c.n_features) * c.n_samples, descending_sort);
			Assert::AreEqual(10.0, c.coords[0]);
			Assert::AreEqual(5.0, c.coords[1]);
			free(data);
		}

		TEST_METHOD(Test_sort_idx)
		{
			double* data = (double*)malloc(sizeof(double) * 2);
			data[0] = 5;
			data[1] = 10;
			vectors c(2, 1, data);
			vectors idx = indices(2);
			std::sort(idx.coords, idx.coords + (idx.n_features) * idx.n_samples, sort_indices(c));
			Assert::AreEqual(1.0, idx.coords[0]);
			Assert::AreEqual(0.0, idx.coords[1]);
			free(data);
		}

		TEST_METHOD(Test_sort_by_idx)
		{
			vectors idx(1, 3);
			idx.coords[0] = 1;
			idx.coords[1] = 2;
			idx.coords[2] = 0;
			vectors d(2, 3);
			for (int i = 0; i < 6; i++)
			{
				d.coords[i] = i;
			}
			sort_by_idx(&d, idx);
			double* t = (double*)malloc(sizeof(double) * 6);
			t[0] = 1.0;
			t[1] = 2.0;
			t[2] = 0.0;
			t[3] = 4.0;
			t[4] = 5.0;
			t[5] = 3.0;
			for (int i = 0; i < 6; i++)
			{
				Assert::AreEqual(t[i], d.coords[i]);
			}
			free(t);
		}

		TEST_METHOD(Test_sort_2)
		{
			vectors val(1, 4);
			val.coords[0] = 2.56;
			val.coords[1] = 10.12;
			val.coords[2] = 3.8;
			val.coords[3] = 1.07;
			vectors vec(2, 4);
			vec.coords[0] = 1.2;
			vec.coords[1] = 2.3;
			vec.coords[2] = 3.4;
			vec.coords[3] = 4.5;
			vec.coords[4] = 6.7;
			vec.coords[5] = 7.8;
			vec.coords[6] = 8.9;
			vec.coords[7] = 9.10;
			sort_two(&val, &vec);
			double* vals = (double*)malloc(sizeof(double) * 4);
			vals[0] = 10.12;
			vals[1] = 3.8;
			vals[2] = 2.56;
			vals[3] = 1.07;
			double* vecs = (double*)malloc(sizeof(double) * 8);
			vecs[0] = 2.3;
			vecs[1] = 3.4;
			vecs[2] = 1.2;
			vecs[3] = 4.5;
			vecs[4] = 7.8;
			vecs[5] = 8.9;
			vecs[6] = 6.7;
			vecs[7] = 9.10;
			for (int i = 0; i < val.n_features; i++)
			{
				Assert::AreEqual(vals[i], val.coords[i]);
				for (int j = 0; j < vec.n_samples; j++)
				{
					Assert::AreEqual(vecs[j * vec.n_features + i], vec.coords[j * vec.n_features + i]);
				}
			}
			free(vals);
			free(vecs);
		}

		TEST_METHOD(Test_cutting_dims)
		{
			vectors vec(2, 4);
			vec.coords[0] = 1.2;
			vec.coords[1] = 2.3;
			vec.coords[2] = 3.4;
			vec.coords[3] = 4.5;
			vec.coords[4] = 6.7;
			vec.coords[5] = 7.8;
			vec.coords[6] = 8.9;
			vec.coords[7] = 9.10;
			vec.leave_n_cols(2);
			vectors v(2, 2);
			v.coords[0] = 1.2;
			v.coords[1] = 2.3;
			v.coords[2] = 6.7;
			v.coords[3] = 7.8;
			Assert::IsTrue(vec == v);
		}
	};
}

namespace test_dunn
{
	TEST_CLASS(test_linkages)
	{
	public:
		TEST_METHOD(Test_single_linkage)
		{
			vectors labels(1, 4);
			labels.coords[0] = 0;
			labels.coords[1] = 0;
			labels.coords[2] = 1;
			labels.coords[3] = 2;
			vectors data(4, 2);
			data.coords[0] = 4.0;
			data.coords[1] = 7.0;
			data.coords[2] = 1.0;
			data.coords[3] = 2.0;
			data.coords[4] = 5.0;
			data.coords[5] = 2.0;
			data.coords[6] = 1.0;
			data.coords[7] = 1.0;
			double d = inter_linkage(labels, data, 0, 1, dist_::Euclidean, inter_::closest);
			Assert::AreEqual(4.0, d);
		}

		TEST_METHOD(Test_complete_linkage)
		{
			vectors labels(1, 4);
			labels.coords[0] = 0;
			labels.coords[1] = 0;
			labels.coords[2] = 1;
			labels.coords[3] = 2;
			vectors data(4, 2);
			data.coords[0] = 4.0;
			data.coords[1] = 7.0;
			data.coords[2] = 1.0;
			data.coords[3] = 2.0;
			data.coords[4] = 5.0;
			data.coords[5] = 2.0;
			data.coords[6] = 1.0;
			data.coords[7] = 1.0;
			double d = inter_linkage(labels, data, 0, 1, dist_::Euclidean, inter_::furthest);
			Assert::AreEqual(sqrt(26.0), d);
		}

		TEST_METHOD(Test_avg_linkage_diff)
		{
			vectors labels(1, 5);
			labels.coords[0] = 0;
			labels.coords[1] = 0;
			labels.coords[2] = 0;
			labels.coords[3] = 1;
			labels.coords[4] = 2;
			vectors d(5, 2);
			d.coords[0] = 1.0;
			d.coords[1] = 2.0;
			d.coords[2] = 3.0;
			d.coords[3] = 4.0;
			d.coords[4] = 5.0;
			d.coords[5] = 6.0;
			d.coords[6] = 7.0;
			d.coords[7] = 8.0;
			d.coords[8] = 9.0;
			d.coords[9] = 10.0;
			double l = inter_linkage(labels, d, 0, 1, dist_::Euclidean, inter_::avg);
			Assert::AreEqual(4 * sqrt(2), l, 1e-6);
		}

		TEST_METHOD(Test_avg_linkage_same)
		{
			kmeans est(3);
			est.labels.change_size(1, 5);
			est.labels.coords[0] = 0;
			est.labels.coords[1] = 0;
			est.labels.coords[2] = 0;
			est.labels.coords[3] = 1;
			est.labels.coords[4] = 2;
			vectors d(5, 2);
			d.coords[0] = 1.0;
			d.coords[1] = 2.0;
			d.coords[2] = 3.0;
			d.coords[3] = 4.0;
			d.coords[4] = 5.0;
			d.coords[5] = 6.0;
			d.coords[6] = 7.0;
			d.coords[7] = 8.0;
			d.coords[8] = 9.0;
			d.coords[9] = 10.0;
			double l = intra_linkage(&est, d, 0, intra_::avg);
			Assert::AreEqual(8 * sqrt(2) / 3, l, 1e-6);
		}
	};

	TEST_CLASS(test_intra)
	{
		TEST_METHOD(Test_intra_furthest)
		{
			kmeans est(2);
			est.labels.change_size(1, 4);
			est.labels.coords[0] = 0;
			est.labels.coords[1] = 0;
			est.labels.coords[2] = 1;
			est.labels.coords[3] = 1;
			vectors data(4, 2);
			data.coords[0] = 10.0;
			data.coords[1] = 7.0;
			data.coords[2] = 0.0;
			data.coords[3] = 0.0;
			data.coords[4] = 5.0;
			data.coords[5] = 2.0;
			data.coords[6] = 1.0;
			data.coords[7] = 3.0;
			double d = intra_distance(&est, data, intra_::furthest);
			Assert::AreEqual(sqrt(149), d);
		}

		TEST_METHOD(Test_intra_avg)
		{
			kmeans est(2);
			est.labels.change_size(1, 5);
			est.labels.coords[0] = 1;
			est.labels.coords[1] = 1;
			est.labels.coords[2] = 1;
			est.labels.coords[3] = 0;
			est.labels.coords[4] = 0;
			vectors d(5, 2);
			d.coords[0] = 1.0;
			d.coords[1] = 2.0;
			d.coords[2] = 3.0;
			d.coords[3] = 4.0;
			d.coords[4] = 5.0;
			d.coords[5] = 6.0;
			d.coords[6] = 7.0;
			d.coords[7] = 8.0;
			d.coords[8] = 9.0;
			d.coords[9] = 10.0;
			double l = intra_distance(&est, d, intra_::avg);
			Assert::AreEqual(8 * sqrt(2) / 3, l, 1e-6);
		}

		TEST_METHOD(Test_avg_dist_to_cent)
		{
			kmeans est(2);
			est.labels.change_size(1, 3);
			est.centroids.change_size(2, 2);
			est.labels.coords[0] = 0;
			est.labels.coords[1] = 1;
			est.labels.coords[2] = 0;
			est.centroids.coords[0] = 2.0;
			est.centroids.coords[1] = 3.0;
			est.centroids.coords[2] = 7.0;
			est.centroids.coords[3] = 8.0;
			vectors d(3, 2);
			d.coords[0] = 1.0;
			d.coords[1] = 2.0;
			d.coords[2] = 10.0;
			d.coords[3] = 11.0;
			d.coords[4] = 3.0;
			d.coords[5] = 4.0;
			double l = intra_centroid(&est, d, 0);
			Assert::AreEqual(sqrt(2), l, 1e-6);
		}

		TEST_METHOD(Test_cent_intra)
		{
			kmeans est(2);
			est.labels.change_size(1, 3);
			est.centroids.change_size(2, 2);
			est.labels.coords[0] = 0;
			est.labels.coords[1] = 1;
			est.labels.coords[2] = 0;
			est.centroids.coords[0] = 2.0;
			est.centroids.coords[1] = 3.0;
			est.centroids.coords[2] = 7.0;
			est.centroids.coords[3] = 8.0;
			vectors d(3, 2);
			d.coords[0] = 1.0;
			d.coords[1] = 2.0;
			d.coords[2] = 10.0;
			d.coords[3] = 11.0;
			d.coords[4] = 3.0;
			d.coords[5] = 4.0;
			Assert::AreEqual(3 * sqrt(2), intra_distance(&est, d, intra_::centroid), 1e-6);
		}
	};

	TEST_CLASS(test_inter)
	{
	public:
		TEST_METHOD(Test_inter_centroid)
		{
			kmeans k(4);
			k.centroids.change_size(4, 2);
			double val = 1.0;
			vectors data(4, 2);
			k.centroids.coords[2] = 1.0;
			k.centroids.coords[3] = 2.0;
			k.centroids.coords[0] = 4.0;
			k.centroids.coords[1] = 7.0;
			k.centroids.coords[4] = 5.0;
			k.centroids.coords[5] = 2.0;
			k.centroids.coords[6] = 1.0;
			k.centroids.coords[7] = 1.0;
			double c = inter_distance(&k, data, inter_::centroid);
			Assert::AreEqual(val, c);
		}
		
		TEST_METHOD(Test_inter_closest)
		{
			kmeans est(3);
			est.labels.change_size(1, 4);
			est.labels.coords[0] = 0;
			est.labels.coords[1] = 0;
			est.labels.coords[2] = 1;
			est.labels.coords[3] = 2;
			vectors data(4, 2);
			data.coords[0] = 4.0;
			data.coords[1] = 7.0;
			data.coords[2] = 1.0;
			data.coords[3] = 2.0;
			data.coords[4] = 5.0;
			data.coords[5] = 2.0;
			data.coords[6] = 1.0;
			data.coords[7] = 1.0;
			double d = inter_distance(&est, data, inter_::closest);
			Assert::AreEqual(1.0, d);
		}

		TEST_METHOD(Test_inter_furthest)
		{
			kmeans est(3);
			est.labels.change_size(1, 4);
			est.labels.coords[0] = 0;
			est.labels.coords[1] = 0;
			est.labels.coords[2] = 1;
			est.labels.coords[3] = 2;
			vectors data(4, 2);
			data.coords[0] = 10.0;
			data.coords[1] = 7.0;
			data.coords[2] = 1.0;
			data.coords[3] = 2.0;
			data.coords[4] = 5.0;
			data.coords[5] = 2.0;
			data.coords[6] = 0.0;
			data.coords[7] = 0.0;
			double d = inter_distance(&est, data, inter_::furthest);
			Assert::AreEqual(sqrt(29), d);
		}
		
		TEST_METHOD(Test_inter_avg)
		{
			kmeans est(3);
			est.labels.change_size(1, 5);
			est.labels.coords[0] = 0;
			est.labels.coords[1] = 0;
			est.labels.coords[2] = 0;
			est.labels.coords[3] = 1;
			est.labels.coords[4] = 2;
			vectors d(5, 2);
			d.coords[0] = 1.0;
			d.coords[1] = 2.0;
			d.coords[2] = 3.0;
			d.coords[3] = 4.0;
			d.coords[4] = 5.0;
			d.coords[5] = 6.0;
			d.coords[6] = 7.0;
			d.coords[7] = 8.0;
			d.coords[8] = 9.0;
			d.coords[9] = 10.0;
			Assert::AreEqual(2 * sqrt(2), inter_distance(&est, d, inter_::avg), 1e-6);
		}
	};
}
