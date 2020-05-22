#include "pch.h"
#include "CppUnitTest.h"
#include "Vectors.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace test
{
	TEST_CLASS(test)
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
			vectors some_vector(2, 10, data);
			vectors other_vector(some_vector);
			Assert::AreEqual(some_vector.coords, other_vector.coords);
			Assert::AreEqual(some_vector.n_samples, other_vector.n_samples);
			Assert::AreEqual(some_vector.n_features, other_vector.n_features);
		}

		TEST_METHOD(Test_dimensions_are_assigned)
		{
			double* data = (double*)malloc(20*sizeof(double));
			vectors some_vector(2, 10, data);
			Assert::AreEqual(some_vector.n_features, 10);
			Assert::AreEqual(some_vector.n_samples, 2);
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
			free(diagonal.coords);
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
		}

		TEST_METHOD(Test_column_length)
		{
			vectors d1 = std_base(2);
			d1.substract(1, 1);
			Assert::AreEqual(length_of_column(d1, 0), 1.0);
			Assert::AreEqual(length_of_column(d1, 1), 1.0);
		}

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
					check_matrix_filling(&C, d3, i, j);
				}
			}
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
		double dot = row_product(v.coords, v.coords+v.n_features, v.n_features);
		Assert::AreEqual(double(7)/ 5, dot);
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
		}

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
			dist = distance(v, v, 0, 1, "cityblock");
			Assert::AreEqual(9.0, dist);
		}

		// double* substract(double value, int row) const
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
			dist = distance(v, v, 0, 1, "correlation");
			Assert::AreEqual(0.0, dist);
		}

		TEST_METHOD(Test_standardised_correlation)
		{

		}
	};
}
