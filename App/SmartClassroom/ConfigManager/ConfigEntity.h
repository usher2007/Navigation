#pragma once

#include <map>
#include <iostream>
#include <string>

const std::string TeacherConfigFile = "..\\Config\\Teacher.conf";
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
	int id;                                    // Global Uinque ID
	int fullScreenLocId;                       // Location ID represent full-screen, usually 0
	double roomWidth;                          // The Room's width, in meters
	double cameraDistance;                     // The distance between camera and rostrum
	int numOfPresetLoc;                        // Count of preset locations
	int presetLocIds[10];                      // Array of preset location ids, max 10
	PresetLocDict presetLocDict;               // The preseted camera locations
	int pixRangeOverlap;                       // The overlap width between two locations, in pixels
	BOOL bShowTracking;                        // Flag denote showing tracking result or not
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