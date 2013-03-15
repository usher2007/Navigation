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
	~CConfigManager();
	// TODO: add your methods here.
	HRESULT LoadConfigFile();
	HRESULT DumpConfigFile();
	// Set Parameters
	HRESULT SetTeacherPresetLoc(int locId, int leftRange, int rightRange);
	HRESULT SetTeacherFullScreen(int locId);
	HRESULT SetTeaShowTracking(BOOL bShowTracking);
	HRESULT SetTeaDetailParams(int pixOverlap, double classroomWidth, double cameraDistance, int leastHumanGap, int humanWidth, int fgLowThresh, 
		int fgUpThresh, double fgHistThresh);
	HRESULT SetTeaTrackingArea(int beginX, int beginY, int beginW, int beginH, int stopX, int stopY, int stopW, int stopH);
	HRESULT SetTeaCommonParams(int disappearFrameThresh, int centerWeightThresh, double gbmLearningRate, int trackingInterval);
	HRESULT SetBlindZone(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
	HRESULT ClearBlindZones();
	// Get Parameters
	HRESULT GetTeaEnvParams(double& roomWidth, double& cameraDistance);
	HRESULT GetTeaPresetLocDict(PresetLocDict** locDict);
	int GetTeaId();
	int GetTeaFullScreenLocId();
	int GetTeaPixRangeOverlap();
	BOOL IsTeaShowTracking();
	HRESULT GetTeaBeginTrackingArea(int& x, int& y, int& width, int& height);
	HRESULT GetTeaStopTrackingArea(int& x, int& y, int& width, int& height);
	int GetTeaLeastHumanGap();
	int GetTeaHumanWidth();
	int GetTeaDisFrameThresh();
	int GetTeaCenterWeightThresh();
	int GetTeaFgLowThresh();
	int GetTeaFgUpThresh();
	double GetTeaFgHistThresh();
	double GetTeaGBMLearningRate();
	int GetTeaTrackingInterval();
	HRESULT GetBlindZoneList(BlindZoneList **bZoneList);
private:
	HRESULT loadTeacherConfig();
	HRESULT loadStudentConfig();
	HRESULT loadLaserPointConfig();
	HRESULT dumpTeacherConfig();
	HRESULT getParamNameAndVal(const std::string& paramLine, std::string& paramName, std::string& paramValue);
	HRESULT setTeaParametersFromFile(const std::string& paramName, std::string& paramValue);
	HRESULT processArrayParameters(const std::string& paramName, std::string& paramValue, int arrayLen);
private:
	TeacherEntity m_teacherEnt;
	StudentEntity m_studentEnt;
	LaserPointEntity m_laserPointEnt;
};

