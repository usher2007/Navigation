#pragma once
#include <iostream>
#include "DShow.h"
#include <initguid.h>
#include <afxwin.h>

class CaptureGraph
{
private:
	HWND owner;
	CComPtr<IGraphBuilder> pGraph;
	CComPtr<ICaptureGraphBuilder2> pCaptureBuilder2;
	CComPtr<IMediaControl> pControl;
	CComPtr<IVideoWindow> pVideoWindow;
	CComPtr<IBasicVideo> pBasicVideo;

public:
	CaptureGraph(void);
	~CaptureGraph(void);

	HRESULT Create(HWND hwnd);
	HRESULT BuildGraph(CString friendlyName);
	HRESULT SetDisplayWindow(HWND windowHwnd);

	HRESULT Stop();
	HRESULT Pause();
	HRESULT Run();

private:
	HRESULT SaveGraphFile(IGraphBuilder *pGraph, WCHAR *wszPath);
};