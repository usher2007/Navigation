#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int GetMinCards(const double inputTotal, vector<double>& cachedResults, vector<double>::iterator endIter);
int main(void)
{
	double inputTotal = 0;
	vector<int> results;
	vector<double> cachedResults;
	vector<double>::iterator endIter;
	cachedResults.reserve(10000);
	cachedResults.push_back(0.0);
	endIter = cachedResults.begin() + 1;
	while(cin >> inputTotal)
	{
		if(inputTotal == 0.0)
		{
			break;
		}
		int offset = endIter - cachedResults.begin();
		int tmpRes = GetMinCards(inputTotal, cachedResults, endIter);
		if(tmpRes > offset)
		{
			endIter = cachedResults.begin() + tmpRes + 1;
		}
		results.push_back(tmpRes);
	}

	vector<int>::iterator resIter;
	for(resIter=results.begin(); resIter!=results.end(); ++resIter)
	{
		cout<<(*resIter)<<" card(s)\n";
	}
	return 0;
}

int GetMinCards(const double inputTotal, vector<double>& cachedResults, vector<double>::iterator endIter)
{
	int res = 0;
	vector<double>::iterator beginIter = cachedResults.begin();
	vector<double>::iterator lowBound = lower_bound(beginIter, endIter,inputTotal);
	if(lowBound == endIter)
	{
		res = endIter - beginIter;
		double totalWeight = *(endIter-1);
		while(totalWeight < inputTotal)
		{
			res++;
			totalWeight += 1.0/res;
			cachedResults.push_back(totalWeight);		
		}
		res--;
	}
	else
	{
		res = lowBound - beginIter;
	}
	return res;
}