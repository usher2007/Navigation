#pragma once
#include "DShow.h"
#include <InitGuid.h>
#include "FilterUIDs.h"
#include <atlcomcli.h>

#ifndef WM_GRAPHNOTIFY
#define WM_GRAPHNOTIFY (WM_APP + 100)
#endif

class CDXFilterGraph
{
public:
	CDXFilterGraph();

	virtual HRESULT Create();
	virtual HRESULT BuildGraph(BOOL display)=0;
	virtual HRESULT SetDisplayWindow(HWND windowHandle);
	virtual HRESULT SetNotifyWindow(HWND windowHandle);

	virtual HRESULT Run();
	virtual HRESULT Stop();

	virtual IMediaEventEx * GetEventHandle();

	virtual HRESULT Destroy();
protected:
	HRESULT init();

protected:
	CComPtr<IGraphBuilder> m_pGraphBuilder;
	CComPtr<IMediaControl> m_pMediaControl;
	CComPtr<IMediaEventEx> m_pMediaEvent;
	CComPtr<IVideoWindow> m_pVideoWindow;
	CComPtr<IBasicVideo> m_pBasicVideo;

	HWND m_hDisplayWnd;
	BOOL m_bDisplay;
};

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

private:
	CComPtr<ITrackingControl> m_pTrackingControl;
};

class CDXFilterGraphTeaPTZ : public CDXFilterGraph
{
public:
	CDXFilterGraphTeaPTZ();

	HRESULT BuildGraph(BOOL bDisplay);
};