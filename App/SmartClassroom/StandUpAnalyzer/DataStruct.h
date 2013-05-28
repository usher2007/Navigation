#include <iostream>
#include <map>

typedef struct _standUpInfo
{
	double slope;
	double weight;
	int pos;
} StandUpInfo;

typedef std::map<int, StandUpInfo> StandUpRowInfo;
typedef std::map<int, StandUpInfo>::iterator StandUpInfoIter;