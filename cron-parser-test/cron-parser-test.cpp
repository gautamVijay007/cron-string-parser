#include "pch.h"
#include "CppUnitTest.h"
#include <vector>
#include "..\cron-parser\CronStringParser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace cronparsertest
{
	TEST_CLASS(cronparsertest)
	{
		CronStringParser cronStringParser;
	public:
		/*
		intVectorToString util method test
		*/
		TEST_METHOD(intVectorToString)
		{
			vector<int> test = { 1,2,3,4 };
			string exp = "1 2 3 4";
			string x = intVectorToStringDup(test);
			Assert::AreEqual(exp, x);
		}

		// Parse string all fields valid
		TEST_METHOD(parseString_Valid)
		{
			auto data = cronStringParser.parseString("*/20 * 1,15,31 3 4-6 /usr/bin/find");
			int actualSize = data.size();
			Assert::AreEqual(6, actualSize);

			bool actualDataPresent = data.find(minute) != data.end() &&
				data.find(hour) != data.end() &&
				data.find(command) != data.end() &&
				data.find(month) != data.end() &&
				data.find(dayOfWeek) != data.end() &&
				data.find(dayOfMonth) != data.end();

			Assert::AreEqual(true, actualDataPresent);

			string actualMinData = data[minute];
			string expectedMinData = "0 20 40";
			Assert::AreEqual(expectedMinData, actualMinData);

			string actualHourData = data[hour];
			string expectedMHourData = "0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23";
			Assert::AreEqual(expectedMHourData, actualHourData);

			string actualDOMData = data[dayOfMonth];
			string expectedMDOMData = "1 15 31";
			Assert::AreEqual(expectedMHourData, actualHourData);

			string actualMonthData = data[month];
			string expectedMonthData = "3";
			Assert::AreEqual(expectedMonthData, actualMonthData);

			string actualDOWData = data[dayOfWeek];
			string expectedDOWData = "4 5 6";
			Assert::AreEqual(expectedDOWData, actualDOWData);

			string actualCommandData = data[command];
			string expectedCommandData = "/usr/bin/find";
			Assert::AreEqual(expectedCommandData, actualCommandData);
		}

		// Invalid Input on Parse field
		TEST_METHOD(ParseString_InvalidInput)
		{
			auto func1 = [this] { cronStringParser.parseString("InvalidInput"); };
			Assert::ExpectException<exception>(func1);

			auto func2 = [this] { cronStringParser.parseString("*/20 * 1,15,32 3 /usr/bin/find"); };
			Assert::ExpectException<exception>(func2);
		}

		// Cron data out of range per field
		TEST_METHOD(ParseString_DataOutOfRange)
		{
			//day of month
			auto func1 = [this] { cronStringParser.parseString("*/20 * 1,15,32 3 4-6 /usr/bin/find"); };
			Assert::ExpectException<exception>(func1);

			//day of week
			auto func2 = [this] { cronStringParser.parseString("*/20 * 1,15,31 3 4-8 /usr/bin/find"); };
			Assert::ExpectException<exception>(func2);

			//Minute
			auto func3 = [this] { cronStringParser.parseString("66 * 1,15,31 3 4-6 /usr/bin/find"); };
			Assert::ExpectException<exception>(func3);

			// Month
			auto func4 = [this] { cronStringParser.parseString("20 12 1,15,31 13 4-6 /usr/bin/find"); };
			Assert::ExpectException<exception>(func4);

			// Hour
			auto func5 = [this] { cronStringParser.parseString("20 29 1,15,31 11 4-6 /usr/bin/find"); };
			Assert::ExpectException<exception>(func5);
		}
	};
}
