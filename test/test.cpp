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
			mean = some_vector.mean(i);
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
			for (int i = 0; i < 2; i++)
			{
				for (int j = 0; j < 2; j++)
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
	};
}
