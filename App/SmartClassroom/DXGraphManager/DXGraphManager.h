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

#include "DXFilterGraphTea.h"
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
	HRESULT StartTeacherTracking();
	HRESULT StopTeacherTracking();
private:
	CDXFilterGraphTea * m_pTeacherGraph;
	CDXFilterGraphTeaPTZ *m_pTeacherPTZGraph;
};

