#pragma once
#include "DShow.h"
#include <InitGuid.h>
#include "FilterUIDs.h"
#include <atlcomcli.h>
#include "DXFilterGraph.h"

#ifndef WM_GRAPHNOTIFY
#define WM_GRAPHNOTIFY (WM_APP + 100)
#endif

class CDXFilterGraphTea : public CDXFilterGraph
{
public:
	CDXFilterGraphTea();

	HRESULT BuildGraph(BOOL bDisplay);

	//
	// --- ITrackingControl Interface --
	//
	HRESULT StartTracking(BOOL bShowTrackingRes);
	HRESULT StopTracking();
	HRESULT EraseCachedVertexes();
	HRESULT CacheAndShowBZoneVertex(int xPix, int yPix);
	HRESULT AddBlindZone(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
	HRESULT ClearBlindZones();
	HRESULT SetFullScreenStrategy(int fullScrMinDur, int noPersonMaxDur);
private:
	//
	// --- Transport the params from Config to Tracking Filter --
	//
	HRESULT syncConfiguration();

private:
	CComPtr<ITrackingControl> m_pTrackingControl;
};

class CDXFilterGraphTeaPTZ : public CDXFilterGraph
{
public:
	CDXFilterGraphTeaPTZ();

	HRESULT BuildGraph(BOOL bDisplay);
};