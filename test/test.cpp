#include "pch.h"
#include "CppUnitTest.h"

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

		TEST_METHOD(TestMethod21)
		{
			Assert::AreEqual(1, 1);
		}
	};
}
