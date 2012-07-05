#pragma once
#include <iostream>
#include "DShow.h"
#include <initguid.h>


// {3C78B8E2-6C4D-11d1-ADE2-0000F8754B99}
DEFINE_GUID(CLSID_WavDest,
0x3c78b8e2, 0x6c4d, 0x11d1, 0xad, 0xe2, 0x0, 0x0, 0xf8, 0x75, 0x4b, 0x99);

class RecordGraph
{
public:
	RecordGraph(void);
	~RecordGraph(void);

	HRESULT Create(HWND hwnd);
	HRESULT BuildGraph(CString recordFileName);
	HRESULT Run();
	HRESULT Stop();
private:
	HWND owner;
	IGraphBuilder *pBuilder;
	IMediaControl *pControl;

	HRESULT CheckMediaType(IEnumMediaTypes *pEnumMedia, GUID MediaType);
	HRESULT ConnectFilters(IGraphBuilder *pGraph, IBaseFilter *pSrc, IBaseFilter *pDest, GUID MediaType);
	HRESULT GetUnconnectedPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir, IPin **ppPin, GUID MediaType);
	HRESULT SaveGraphFile(IGraphBuilder *pGraph, WCHAR *wszPath);

};