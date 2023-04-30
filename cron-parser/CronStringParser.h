#include <iostream>
#include <sstream>
#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <regex>
#include "utils.h"

using namespace std;

enum CronFieldType
{
	minute,
	hour,
	dayOfMonth,
	month,
	dayOfWeek,
	command
};

class CronFieldDetails {
public:
	string rawString;
	CronFieldType cronFieldType;
};

const vector<pair<CronFieldType, string>> defaultCronFieldOrder = { {minute, "minute"} , {hour, "hour"}, {dayOfMonth, "day of month"}, {month, "month"}, {dayOfWeek, "day of week"}, {command, "command"} };

unordered_map<CronFieldType, pair<int, int>> cronRanges =
{
	{minute, make_pair(0, 59)},
	{hour, make_pair(0, 23)},
	{dayOfMonth, make_pair(1, 31)},
	{month, make_pair(1, 12)},
	{dayOfWeek,make_pair(0, 6)}
};

class CronStringParser
{
	vector<pair<CronFieldType, string>> cronFieldsOrder;

	bool isValidCron(string cronString)
	{
		regex b("([0-9]{1,2}|[*])([,/-][0-9]{1,2})*|[*]|[?]$");
		return regex_match(cronString, b);
	}

	/*
	Expand field and return time as per the field type at which it will run
	@param cronItem object that will hold rawString and cronFieldType
	*/
	vector<int> expandCronItem(CronFieldDetails& cronItem)
	{
		vector<int> res;
		pair<int, int> range = cronRanges[cronItem.cronFieldType];

		// Cron String Validation
		if (!isValidCron(cronItem.rawString))
		{
			cout << "Invalid Cron String at " << cronFieldsOrder[cronItem.cronFieldType].second << endl;
			throw exception("Invalid Cron String");
		}

		// Case handling for cron item expansion
		try {
			if (cronItem.rawString.find('/') != string::npos)
			{
				int start = cronItem.rawString[0] == '*' ? range.first : stoi(cronItem.rawString.substr(0, cronItem.rawString.find('/')));
				int interval = stoi(cronItem.rawString.substr(cronItem.rawString.find('/') + 1));
				if (!isInRange(start, range.first, range.second))
				{
					throwRangeExceptionError(cronFieldsOrder[cronItem.cronFieldType].second);
				}
				while (start <= range.second)
				{
					res.push_back(start);
					start += interval;
				}
			}
			else if (cronItem.rawString == "?" && (cronItem.cronFieldType == dayOfMonth || cronItem.cronFieldType == dayOfWeek))
			{
				return res;
			}
			else if (cronItem.rawString == "*")
			{
				for (int it = range.first; it <= range.second; it++)
				{
					res.push_back(it);
				}
			}
			else if (cronItem.rawString.find(',') != string::npos)
			{
				string rawValue;
				for (int i = 0; i < cronItem.rawString.length(); i++)
				{
					if (cronItem.rawString[i] == ',' || i == cronItem.rawString.length() - 1)
					{
						try
						{
							if (i == cronItem.rawString.length() - 1)
							{
								rawValue += cronItem.rawString[i];
							}
							int value = stoi(rawValue);
							if (!isInRange(value, range.first, range.second))
							{
								throwRangeExceptionError(cronFieldsOrder[cronItem.cronFieldType].second);
							}
							res.push_back(value);
							rawValue = "";
						}
						catch (const std::exception&)
						{
							throw exception("Incorrect Input");
						}
					}
					else
					{
						rawValue += cronItem.rawString[i];
					}
				}
			}
			else if (cronItem.rawString.find('-') != string::npos)
			{
				int delimeterInd = cronItem.rawString.find('-');
				int start = stoi(cronItem.rawString.substr(0, delimeterInd));
				int end = stoi(cronItem.rawString.substr(delimeterInd + 1));

				if (!isInRange(start, range.first, range.second) || !isInRange(end, range.first, range.second) || start > end)
				{
					throwRangeExceptionError(cronFieldsOrder[cronItem.cronFieldType].second);
				}

				for (int it = start; it <= end; it++)
				{
					res.push_back(it);
				}

			}
			else
			{
				int value = stoi(cronItem.rawString);
				if (!isInRange(value, range.first, range.second))
				{
					throwRangeExceptionError(cronFieldsOrder[cronItem.cronFieldType].second);
				}
				res.push_back(value);
			}
		}
		catch (const std::exception&)
		{

			cout << "Incorrect Input at " + cronFieldsOrder[cronItem.cronFieldType].second;
			throw exception("Incorrect Input");
		}

		return res;
	}

	void throwRangeExceptionError(string field )
	{
		cout << "Incorrect Input on " + field + ": value Out of Range";
		throw exception("Incorrect Input: value out of range");
	}

	void splitCronFields(string rawString, vector<CronFieldDetails>& cronFieldList)
	{
		CronFieldDetails* temp = new CronFieldDetails();

		// Split string into vector of cronDetail objects
		for (int i = 0; i < rawString.length(); i++)
		{
			if (rawString[i] == ' ' && temp->rawString != "")
			{
				cronFieldList.push_back(*temp);
				temp = new CronFieldDetails();
			}
			else
			{
				temp->rawString += rawString[i];
			}
		}

		// To handle the last string case. Append last string in the list 
		if (temp->rawString != "")
		{
			cronFieldList.push_back(*temp);
		}
		free(temp);
	}

public:
	/*
	Constructor that sets the cronFieldOrder in which strings are provided
	@param order accepts all the cron fields order should present in the string
	*/
	CronStringParser(vector<pair<CronFieldType, string>> order = defaultCronFieldOrder) {
		cronFieldsOrder = order;
	}

	/*
	Parses string and split each field detail in cronFieldDetail object
	*/
	unordered_map<CronFieldType, string> parseString(char* argv)
	{
		// Data Parsing
		string rawString = argv;
		vector<CronFieldDetails> cronFieldList;
		splitCronFields(rawString, cronFieldList);

		// Pre-Validation
		if (cronFieldList.size() != cronFieldsOrder.size())
		{
			cout << "Invalid Input length";
			throw exception("Invalid Input length");
		}

		// Value Expansion
		unordered_map<CronFieldType, string> fieldToValueMapping;
		for (int i = 0; i < cronFieldList.size(); i++)
		{
			cronFieldList[i].cronFieldType = cronFieldsOrder[i].first;
			if (cronFieldList[i].cronFieldType != command)
			{
				fieldToValueMapping[cronFieldsOrder[i].first] = intVectorToString(expandCronItem(cronFieldList[i]));
			}
			else
			{
				fieldToValueMapping[cronFieldsOrder[i].first] = cronFieldList[i].rawString;
			}
		}

		return fieldToValueMapping;
	}

	/*
	Print the data formatted as a table with the field name taking the first 14 columns and
	the times as a space-separated list following it
	*/
	void printData(unordered_map<CronFieldType, string> fieldToValueMapping)
	{
		// Iterate cron fields in given order
		for (auto cronField : cronFieldsOrder)
		{
			// Print the Label for field in 14 static position 
			cout << cronField.second + string(14 - cronField.second.length(), ' ');
			// Print the value for field type
			cout << fieldToValueMapping[cronField.first] << endl;
		}
	}
};