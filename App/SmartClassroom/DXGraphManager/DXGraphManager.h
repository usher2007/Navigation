// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DXGRAPHMANAGER_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DXGRAPHMANAGER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef DXGRAPHMANAGER_EXPORTS
#define DXGRAPHMANAGER_API __declspec(dllexport)
#else
#define DXGRAPHMANAGER_API __declspec(dllimport)
#endif
#pragma once
#include "DXFilterGraphTea.h"
#include "DXFilterGraphStu.h"
// This class is exported from the DXGraphManager.dll
class DXGRAPHMANAGER_API CDXGraphManager {
public:
	CDXGraphManager(void);
	HRESULT CreateTeacherGraph(BOOL bDisplay, HWND displayWnd, HWND notifyWnd);
	HRESULT CreateTeacherPTZGraph(BOOL bDisplay, HWND displayWnd, HWND notifyWnd);
	HRESULT RunTeacherGraph();
	HRESULT RunTeacherPTZGraph();
	HRESULT StopTeacherGraph();
	HRESULT StopTeacherPTZGraph();
	HRESULT StartTeacherTracking(BOOL bShowTrackingRes);
	HRESULT StopTeacherTracking();
	HRESULT EraseCachedTeacherBZoneVertex();
	HRESULT CacheAndShowTeacherBZoneVertex(int xPix, int yPix);
	HRESULT AddTeacherBlindZone(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4); 
	HRESULT ClearBlindZones();
	HRESULT SetTeacherFullScrStrategy(int fullScrMinDur, int noPersonMaxDur);

	HRESULT CreateStudentGraph(BOOL bDisplay, HWND displayWnd, HWND notifyWnd, int number);
	HRESULT CreateStudentPTZGraph(BOOL bDisplay, HWND displayWnd, HWND notifyWnd);
	HRESULT RunStudentGraph(int number);
	HRESULT RunStudentPTZGraph();
	HRESULT StopStudentGraph(int number);
	HRESULT StopStudentPTZGraph();
	HRESULT StartStandUpDectect(BOOL bShowDetectRes);
	HRESULT StopStandUpDectect();
	HRESULT SetStuStandUpParams(int leftBorder, int rightBorder, int totalRowNum, int totalColNum, int detectLine);

private:
	CDXFilterGraphTea * m_pTeacherGraph;
	CDXFilterGraphTeaPTZ *m_pTeacherPTZGraph;

	CDXFilterGraphStu * m_pStudentGraphLeft;
	CDXFilterGraphStu * m_pStudentGraphRight;
	CDXFilterGraphStuPTZ * m_pStudentPTZGraph;
};

