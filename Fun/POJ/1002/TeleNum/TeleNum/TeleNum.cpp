#include <iostream>
#include <iomanip>
#include <set>
#include <string>

using namespace std;

int string2teleNum(string s);

int main(void)
{
	int numOfLines, tmpTeleNum;
	int *TeleNumbers = new int[10000000]();
	memset(TeleNumbers, 0x00, 10000000*sizeof(int));

	set<int> dupTeleNumbers;
	string s;

	if(!(cin>>numOfLines))
	{
		return -1;
	}

	for(int i=0; i<numOfLines; ++i)
	{
		cin>>s;
		tmpTeleNum = string2teleNum(s);
		++TeleNumbers[tmpTeleNum];
		if(TeleNumbers[tmpTeleNum] > 1)
		{
			dupTeleNumbers.insert(tmpTeleNum);
		}
	}
	if(dupTeleNumbers.empty())
	{
		cout<<"No duplicates.\r\n";
		return 0;
	}
	set<int>::iterator it;
	int firstThree = 0, lastFour = 0;
	for(it=dupTeleNumbers.begin(); it!=dupTeleNumbers.end(); ++it)
	{
		tmpTeleNum = *it;
		firstThree = tmpTeleNum /10000;
		lastFour = tmpTeleNum - firstThree*10000;
		cout<<setfill('0')<<setw(3)<<firstThree<<setw(1)<<"-"<<setw(4)<<lastFour<<setw(1)<<" "<<TeleNumbers[tmpTeleNum]<<"\r\n";
	}
	return 0;
}

int string2teleNum(string s)
{
	int res = 0;
	char c;
	int tmp;
	for(int i=0; i<s.length(); ++i)
	{
		c = s[i];
		switch(c)
		{
		case '0':
			tmp = 0; break;
		case '1':
			tmp = 1; break;
		case '2':
		case 'A':
		case 'B':
		case 'C':
			tmp = 2; break;
		case '3':
		case 'D':
		case 'E':
		case 'F':
			tmp = 3; break;
		case '4':
		case 'G':
		case 'H':
		case 'I':
			tmp = 4; break;
		case '5':
		case 'J':
		case 'K':
		case 'L':
			tmp = 5; break;
		case '6':
		case 'M':
		case 'N':
		case 'O':
			tmp = 6; break;
		case '7':
		case 'P':
		case 'R':
		case 'S':
			tmp = 7; break;
		case '8':
		case 'T':
		case 'U':
		case 'V':
			tmp = 8; break;
		case '9':
		case 'W':
		case 'X':
		case 'Y':
			tmp = 9; break;
		default:
			tmp = -1; break;
		}
		if(tmp == -1)
			continue;
		res = res * 10 + tmp;
	}
	return res;
}