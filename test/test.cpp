#include "pch.h"
#include "CppUnitTest.h"
#include "Header.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace test
{
	TEST_CLASS(test)
	{
	public:
		TEST_METHOD(TestMethod1)
		{
			Assert::AreNotEqual(1, 2);
		}

		TEST_METHOD(TestMethod2)
		{
			Assert::AreEqual(1, 1);
		}

		TEST_METHOD(TestMethod3)
		{
			Assert::AreEqual(ret_1(), 1);
		}
	};
}
