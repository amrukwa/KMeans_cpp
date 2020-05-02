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

		TEST_METHOD(Test_values_are_assigned)
		{
			double* data = (double*)malloc(20*sizeof(double));
			vectors some_vector(2, 10, data);
			Assert::AreEqual(some_vector.n_features, 10);
			Assert::AreEqual(some_vector.n_samples, 2);
			Assert::AreEqual(some_vector.coords, data);
		}


	};
}
