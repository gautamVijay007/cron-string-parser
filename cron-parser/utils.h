#include <vector>
#include <string>

using namespace std;

/*
	returns space separated string from int array element
*/
string intVectorToString(vector<int> arr)
{
	string result;
	for (int i = 0; i < arr.size(); i++)
	{
		result += to_string(arr[i]);
		if (i != arr.size() - 1)
		{
			result += " ";
		}
	}

	return result;
}

/*
	returns whether value is in between start and end(both inclusive)
*/
bool isInRange(int value, int start, int end)
{
	return value >= start && value <= end;
}
