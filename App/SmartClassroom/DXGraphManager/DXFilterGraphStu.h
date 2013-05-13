#pragma once
#include "DXFilterGraph.h"
#include "DShow.h"
#include <InitGuid.h>
#include "FilterUIDs.h"
#include <atlcomcli.h>

#ifndef WM_GRAPHNOTIFY
#define WM_GRAPHNOTIFY (WM_APP + 100)
#endif

class CDXFilterGraphStu : public CDXFilterGraph
{
public:
	CDXFilterGraphStu(int posNum);

	HRESULT BuildGraph(BOOL bDisplay);

	HRESULT StartDetect(BOOL bShowDetectRes);
	HRESULT StopDetect();

private:
	int num;
};

class CDXFilterGraphStuPTZ : public CDXFilterGraph
{
public:
	CDXFilterGraphStuPTZ();

	HRESULT BuildGraph(BOOL display);
};