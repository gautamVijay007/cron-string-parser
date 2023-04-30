// cron-parser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>
#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include "CronStringParser.h"

using namespace std;

int main(int argc, char* args[])
{
	if (argc == 2) {

		CronStringParser* cronStringPareser = new CronStringParser();

		unordered_map<CronFieldType, string> data = cronStringPareser->parseString(args[1]);

		cronStringPareser->printData(data);
	}
	else
	{
		cout << "Invalid Arguments: Valid Cron String not Found";
		throw "Invalid Arguments: Valid Cron String not Found";
	}

	return 0;
}
