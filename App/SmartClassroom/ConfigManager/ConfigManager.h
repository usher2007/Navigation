// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CONFIGMANAGER_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// CONFIGMANAGER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef CONFIGMANAGER_EXPORTS
#define CONFIGMANAGER_API __declspec(dllexport)
#else
#define CONFIGMANAGER_API __declspec(dllimport)
#endif
#pragma once
#include <vector>
#include "ConfigEntity.h"

// This class is exported from the ConfigManager.dll
class CONFIGMANAGER_API CConfigManager {
public:
	CConfigManager(void);
	// TODO: add your methods here.
	HRESULT LoadConfigFile();
	HRESULT DumpConfigFile();
	HRESULT SetTeacherPresetLoc(int locId, int leftRange, int rightRange);
	HRESULT SetTeacherFullScreen(int locId);
	HRESULT SetTeaEnvParams(double roomWidth, double cameraDistance);
	HRESULT SetTeaShowTracking(BOOL bShowTracking);
	HRESULT GetTeaEnvParams(double& roomWidth, double& cameraDistance);
	HRESULT GetTeaPresetLocDict(PresetLocDict** locDict);
	int GetTeaId();
	int GetTeaFullScreenLocId();
	int GetTeaPixRangeOverlap();
	BOOL IsTeaShowTracking();
private:
	HRESULT loadTeacherConfig();
	HRESULT loadStudentConfig();
	HRESULT loadLaserPointConfig();
	HRESULT getParamNameAndVal(const std::string& paramLine, std::string& paramName, std::string& paramValue);
	HRESULT setTeaParametersFromFile(const std::string& paramName, std::string& paramValue);
private:
	TeacherEntity m_teacherEnt;
	StudentEntity m_studentEnt;
	LaserPointEntity m_laserPointEnt;
};

