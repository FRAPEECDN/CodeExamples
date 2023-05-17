#include "pch.h"
#include "CppUnitTest.h"
#include <string>
#include <memory>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestExample
{
	struct ExampleStruct {
		int a{};
		std::string b{};
		ExampleStruct() {}
	};

	int addExample(int a, int b) {
		return a + b;
	}


	TEST_CLASS(UnitTestExample)
	{
	public:
		
		TEST_CLASS_INITIALIZE(ClassInit)
		{

		}

		TEST_CLASS_CLEANUP(ClassCleanup)
		{

		}

		TEST_METHOD(TestMethod1)
		{
			Assert::IsTrue(addExample(1, 2) == 3);
			Assert::IsFalse(addExample(1, 2) == 4);
		}

		TEST_METHOD(TestMethod2)
		{
			std::unique_ptr<std::string> p = std::make_unique<std::string>();
			*p = "Hello";
			std::string expected = "Hello";
			Assert::AreEqual(expected.compare(*p), 0);
		}	
		
		TEST_METHOD(TestMethod3)
		{
			std::unique_ptr<std::string> p = std::make_unique<std::string>();
			*p = "Hello";
			std::string notExpected = "Bye";
			Assert::AreNotEqual(notExpected.compare(*p), 0);
		}

		TEST_METHOD(TestMethod4) {
			int a = 3;
			int b = 4;
			Assert::AreNotEqual(a, b);
			b = 3;
			Assert::AreEqual(a, b);
		}

		TEST_METHOD(TestMethod5) {
			std::unique_ptr<ExampleStruct> p = std::make_unique<ExampleStruct>();
			std::string expected = "";
			Assert::AreEqual(p->a, 0);
			Assert::AreEqual(p->b, expected);
		}
	};
}
