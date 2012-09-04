#include <iostream>
#include <string>

using namespace std;
int addTwoUnitNum(const char first, const char second, int carry, char *res);
int multipTwoUnitNum(const char first, const char second, int carry, char *res);
string addTwoNum(string first, string second);
string multipOneNumWithOneUnitNum(string first, const char second);
string multipTwoNum(string first, string second);
string addTwoNumWithShift(string first, string second, int shiftNum);
string trimRes(string res);
int main(void)
{
	string s, tmpRes, finalRes;
	int n;
	finalRes = "";
	while(cin>>s>>n)
	{
		tmpRes = "1";
		for(int i=0; i<n; i++)
		{
			tmpRes = multipTwoNum(tmpRes, s);
		}
		tmpRes = trimRes(tmpRes);
		finalRes.append(tmpRes + "\r\n");
		tmpRes.clear();
	}
	cout<<finalRes;
	return 0;
}

string trimRes(string res)
{
	int decimalPointPos = res.find('.');
	if(decimalPointPos >= 0)
	{
		for(int i=0; i<decimalPointPos; i++)
		{
			if(res[i] == '0')
			{
				res.erase(i, 1);
				i--;
				decimalPointPos--;
			}
			else
			{
				break;
			}
		}
		int len = res.length();
		int j=len-1;
		for(; j>decimalPointPos; j--)
		{
			if(res[j] == '0')
			{
				res.erase(j, 1);
			}
			else
			{
				break;
			}
		}
		if(j == decimalPointPos)
		{
			res.erase(decimalPointPos, 1);
		}
	}
	else
	{
		int len = res.length();
		for(int i=0; i<len; i++)
		{
			if(res[i] == '0')
			{
				res.erase(i, 1);
				i--;
				len--;
			}
			else
			{
				break;
			}
		}
	}
	return res;
}

string multipTwoNum(string first, string second)
{
	int firstLen = first.length();
	int secondLen = second.length();
	int firstPointPos = first.find('.');
	int secondPointPos = second.find('.');
	int firstDecimalLen = 0;
	int secondDecimalLen = 0;
	if(firstPointPos >= 0)
	{
		 firstDecimalLen= firstLen - firstPointPos - 1;
		 first.erase(firstPointPos, 1);
		 firstLen --;
	}
	if(secondPointPos >= 0)
	{
		secondDecimalLen = secondLen - secondPointPos - 1;
		second.erase(secondPointPos, 1);
		secondLen --;
	}
	string finalRes = "0";
	string tmpRes;
	int shiftNum = 0;
	for(int i=secondLen-1; i>=0; i--)
	{
		tmpRes = multipOneNumWithOneUnitNum(first, second[i]);
		finalRes = addTwoNumWithShift(finalRes, tmpRes, shiftNum);
		shiftNum ++;
	}
	finalRes.insert(finalRes.length() - (firstDecimalLen + secondDecimalLen), 1, '.');
	return finalRes;
}

string multipOneNumWithOneUnitNum(string first, const char second)
{
	int len = first.length();
	char firstEle = '0';
	char eachRes = '0';
	int carry = 0;
	string res;
	for(int i=len-1; i>=0; i--)
	{
		firstEle = first[i];
		carry = multipTwoUnitNum(firstEle, second, carry, &eachRes);
		res.insert(res.begin(), eachRes);
	}
	if(carry > 0)
	{
		res.insert(res.begin(), carry + '0');
	}
	return res;
}

string addTwoNum(string first, string second)
{
	int firstLen = first.length();
	int secondLen = second.length();
	int firstIndex = firstLen - 1, secondIndex = secondLen - 1;
	int carry = 0;
	string finalRes = "";
	char eachRes;
	while(firstIndex >= 0 || secondIndex >= 0)
	{
		char firstUnit = firstIndex >= 0 ? first[firstIndex] : '0';
		char secondUnit = secondIndex >= 0 ? second[secondIndex] : '0';
		carry = addTwoUnitNum(firstUnit, secondUnit, carry, &eachRes);
		finalRes.insert(finalRes.begin(),eachRes);
		firstIndex --;
		secondIndex --;
	}
	if(carry > 0)
	{
		finalRes.insert(finalRes.begin(), carry + '0');
	}
	return finalRes;
}

string addTwoNumWithShift(string first, string second, int shiftNum)
{
	if(shiftNum > 0)
	{
		second.append(shiftNum, '0');
	}
	return addTwoNum(first, second);
}

int multipTwoUnitNum(const char first, const char second, int carry, char *res)
{
	int tmpResultInt = (first - '0') * (second - '0') + carry;
	int unitRes = tmpResultInt % 10;
	int decadeRes = tmpResultInt / 10;
	*res = unitRes + '0';
	return decadeRes;
}

int addTwoUnitNum(const char first, const char second, int carry, char *res)
{
	int tmpResultInt = (first - '0') + (second - '0') + carry;
	int unitRes = tmpResultInt % 10;
	int decadeRes = tmpResultInt / 10;
	*res = unitRes + '0';
	return decadeRes;
}