#include <iostream>
#include <cmath>
#include <windows.h>

static int totalMSeconds = 1000;
static int busyWeight[200];
static int sampleNum = 200;
static double PI = 3.1415926;
static int offset = 50;
static int amp = 40;
static int idleMSeconds = totalMSeconds;

int main(void)
{
	for(int i=0; i<sampleNum; ++i)
	{
		busyWeight[i] = (offset + amp*sin(i*2*PI/200))*totalMSeconds/100;
	}
	int j=0;
	while(true)
	{
		int ticBegin = GetTickCount();
		while(GetTickCount() - ticBegin < busyWeight[j])
		{

		}
		Sleep(totalMSeconds - busyWeight[j]);
		j++;
		j = j%200;
	}
}