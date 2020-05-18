#include "pch.h"
#include "CppUnitTest.h"
#include "Vectors.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace test
{
	TEST_CLASS(test)
	{
	public:
		TEST_METHOD(Test_vector_is_copied)
		{
			double* data = (double*)malloc(20 * sizeof(double));
			vectors some_vector(2, 10, data);
			vectors other_vector = some_vector;
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

		void check_mean_of_row(vectors some_vector, int i)
		{
			Assert::AreEqual(some_vector.mean_of_vector(i), 0.5 + 2*i);
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

		TEST_METHOD(Test_mean_of_row)
		{
			double* data;
			data = (double*)malloc(sizeof(double) * 4);
			for (int i = 0; i < 4; i++)
			{
				data[i] = double(i);
			}
			vectors d1(2, 2, data);
			for (int i = 0; i < d1.n_samples; i++)
			{
				check_mean_of_row(d1, i);
			}
		}
	};
}
