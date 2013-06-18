#pragma once
#include <iostream>
#include <map>
#include <queue>

typedef struct _standUpInfo
{
	std::queue<double> cachedSlope;
	double slope;
	double weight;
	int pos;
} StandUpInfo;

typedef std::map<int, StandUpInfo> StandUpRowInfo;
typedef std::map<int, StandUpInfo>::iterator StandUpInfoIter;