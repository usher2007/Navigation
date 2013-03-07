// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the APICONTROLLER_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// APICONTROLLER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef APICONTROLLER_EXPORTS
#define APICONTROLLER_API __declspec(dllexport)
#else
#define APICONTROLLER_API __declspec(dllimport)
#endif
#pragma once
#include "ModuleFactory.h"

// This class is exported from the APIController.dll
class APICONTROLLER_API CAPIController {
public:
	static CAPIController* GetInstance();
	HRESULT DumpConfiguration();


	// Teacher Part
	HRESULT BuildTeacherGraph(BOOL bDisplay, HWND displayWnd, HWND notifyWnd);
	HRESULT BuildTeacherPTZGraph(BOOL bDisplay, HWND displayWnd, HWND notifyWnd);

	HRESULT AddTeaCamera();
	HRESULT TeacherPTZUp();
	HRESULT TeacherPTZDown();
	HRESULT TeacherPTZLeft();
	HRESULT TeacherPTZRight();
	HRESULT TeacherPTZZoomIn();
	HRESULT TeacherPTZZoomOut();
	HRESULT TeacherPTZStop();

	HRESULT TeacherPTZSetPrePos(int locId, int pixLeft, int pixRight, double realLeft, double realRight);
	HRESULT TeacherPTZRecallPrePos(int locId);

	HRESULT TeacherPTZGraphRun();
	HRESULT TeacherPTZGraphStop();

	HRESULT TeacherGraphRun();
	HRESULT TeacherGraphStop();

	HRESULT TeacherTrackingStart();
	HRESULT TeacherTrackingStop();

	HRESULT TeacherGetEnvParams(double& roomWidth, double& cameraDistance);

	HRESULT TeacherSetShowTracking(BOOL bShowTracking);
	HRESULT TeacherSetDetailParams(int pixOverlap, double classroomWidth, double cameraDistance, int leastHumanGap, int humanWidth, int fgLowThresh, 
		int fgUpThresh, double fgHistThresh);
	HRESULT TeacherSetTrackingArea(int beginX, int beginY, int beginW, int beginH, int stopX, int stopY, int stopW, int stopH);
	HRESULT TeacherSetCommonParams(int disappearFrameThresh, int centerWeightThresh, double gbmLearningRate, int trackingInterval);

	HRESULT TeacherCachePointToShow(int xPix, int yPix);
	HRESULT TeacherEraseCurrentBlindZone();
	HRESULT TeacherSaveBlindZone(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
	// Student Part
	HRESULT BuildStudentGraph();

	// LaserPoint Part
	HRESULT BuildLaserPointGraph();

private:
	CAPIController(void);
	~CAPIController();

	HRESULT addCamera(int cameraId, int comNum, int baudRate);
	HRESULT restoreCameraPresetLoc(int cameraId);

private:
	static CAPIController* m_pInstance;
	CModuleFactory* m_pModuleFactory;
	// TODO: add your methods here.
};

