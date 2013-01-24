#pragma once

#include <map>

typedef struct LocRange
{
	int left;
	int right;
} LocRange;

typedef std::map<int, LocRange> PresetLocDict;

class TeacherEntity
{
public:
	int id;
	int fullScreenLocId;
	double roomWidth;
	double cameraDistance;
	PresetLocDict presetLocDict; 
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