#pragma once

#include <map>

typedef struct LocRange
{
	int left;
	int right;
} LocRange;

typedef std::map<int, LocRange> PresetLocDict;
typedef std::map<int, LocRange>::iterator PresetLocDictIter;

class TeacherEntity
{
public:
	int id;
	int fullScreenLocId;
	double roomWidth;
	double cameraDistance;
	PresetLocDict presetLocDict; 
	int pixRangeOverlap;
	BOOL bShowTracking;
};

class StudentEntity
{
public:
	int id;
};

class LaserPointEntity
{
public:
	int id;
};