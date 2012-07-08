#pragma once
#include <iostream>
#include "DShow.h"
#include <initguid.h>

#include <qnetwork.h>
#include <wmcodecdsp.h>
#include  <afxpriv.h>

//#include "IString.h"

//#include <Wmcodecdsp.h>
#define __IDxtCompositor_INTERFACE_DEFINED__

#define __IDxtAlphaSetter_INTERFACE_DEFINED__

#define __IDxtJpeg_INTERFACE_DEFINED__

#define __IDxtKey_INTERFACE_DEFINED__
#ifndef WM_GRAPHNOTIFY
#define WM_GRAPHNOTIFY (WM_APP + 100)
#endif
#ifndef OUR_GUID_ENTRY
#define OUR_GUID_ENTRY(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
	DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8);
#endif

class CaptureGraph
{
private:
	HWND owner;
	IGraphBuilder *pGraph;
	ICaptureGraphBuilder2 *pCaptureBuilder2;
	IMediaControl *pControl;
	IMediaEventEx *pEvent;
	IMediaSeeking *pSeeking;
	IVideoWindow *mVideoWindow;
	BOOL bFullScreen;
	IBasicVideo *pBasicVideo;
	

public:
	CaptureGraph(void);
	~CaptureGraph(void);

	HRESULT Stop();
	HRESULT Pause();
	HRESULT SetNotifyWindow(HWND hwnd);
	HRESULT Create(HWND hwnd);
	HRESULT BuildGraphFromListBox(CListBox *captureFilterList, int selectedIndex);
	HRESULT SetDisplayWindow(HWND hwnd);
	IMediaEventEx* GetEventHandle();
	HRESULT Run();
	HRESULT FullScreen(HWND hwnd);
	HRESULT ConnectFilters(IGraphBuilder *pGraph, IBaseFilter *pSrc, IBaseFilter *pDest, GUID MediaType);
	HRESULT GetUnconnectedPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir, IPin **ppPin, GUID MediaType);
	BOOL IsFullScreen();
	HRESULT EscFullScreen(HWND hwnd);
	void OnSize();
	HRESULT GetState(OAFilterState* pfs);
	HRESULT GetVideoSize(long* pWidth, long* pHeight);
	HRESULT GetCaptureFilters(int *filterCount, CListBox *captureFilterList);

private:
	HRESULT CheckMediaType(IEnumMediaTypes *pEnumMedia, GUID MediaType);
	HRESULT SaveGraphFile(IGraphBuilder *pGraph, WCHAR *wszPath);

};

