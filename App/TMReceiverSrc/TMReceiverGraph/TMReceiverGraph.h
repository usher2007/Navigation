// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the TMRECEIVERGRAPH_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TMRECEIVERGRAPH_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef TMRECEIVERGRAPH_EXPORTS
#define TMRECEIVERGRAPH_API __declspec(dllexport)
#else
#define TMRECEIVERGRAPH_API __declspec(dllimport)
#endif

#pragma once
#include "stdafx.h"
#include "DShow.h"
#include <initguid.h>
#include <atlcomcli.h>
#include "..\TMReceiver\TMReceiver.h"

#ifndef WM_GRAPHNOTIFY
#define WM_GRAPHNOTIFY (WM_APP + 100)
#endif


// {7C85656E-D45D-4C6B-A825-4DF103639DD2}
DEFINE_GUID(CLSID_TMReceiverSrc, 
	0x7c85656e, 0xd45d, 0x4c6b, 0xa8, 0x25, 0x4d, 0xf1, 0x3, 0x63, 0x9d, 0xd2);

// {C1F400A4-3F08-11D3-9F0B-006008039E37}
DEFINE_GUID(CLSID_NullRenderer, 
	0xc1f400a4, 0x3f08, 0x11d3, 0x9f, 0x0b, 0x00, 0x60, 0x08, 0x03, 0x9e, 0x37);

// {1E3C41C2-3872-44CB-83E4-3D14EE823688}
DEFINE_GUID(IID_IRecordStream, 
	0x1e3c41c2, 0x3872, 0x44cb, 0x83, 0xe4, 0x3d, 0x14, 0xee, 0x82, 0x36, 0x88);
MIDL_INTERFACE("1E3C41C2-3872-44CB-83E4-3D14EE823688")
IRecordStream : public IUnknown
{
	virtual HRESULT StartRecord(const char* fileName)PURE;
	virtual HRESULT StopRecord()PURE;
};

// {E74BC5A9-44AE-4AC4-8B26-6FE694940EA5}
DEFINE_GUID(IID_ISetCallBack, 
	0xe74bc5a9, 0x44ae, 0x4ac4, 0x8b, 0x26, 0x6f, 0xe6, 0x94, 0x94, 0xe, 0xa5);
MIDL_INTERFACE("E74BC5A9-44AE-4AC4-8B26-6FE694940EA5")
ISetCallBack : public IUnknown
{
	virtual HRESULT SetCallBackBeforeDecode(TMReceiverCB* cb, void* arg)PURE;
	virtual HRESULT SetCallBackAfterDecode(TMReceiverCB* cb, void* arg)PURE;
};

// This class is exported from the TMReceiverGraph.dll
class TMRECEIVERGRAPH_API CTMReceiverGraph {
public:
	CTMReceiverGraph(void);
	~CTMReceiverGraph();

	HRESULT Create();
	HRESULT BuildFilterGraph(const char* fileName, BOOL bDisplay);
	HRESULT SetDisplayWindow(HWND windowHandle);
	HRESULT SetNotifyWindow(HWND windowHandle);
	HRESULT SetBeforeDecodeCB(TMReceiverCB* cb, void* arg);
	HRESULT SetAfterDecodeCB(TMReceiverCB* cb, void* arg);
	void OnSize();

	HRESULT Stop();
	HRESULT Pause();
	HRESULT Run();

	HRESULT StartRecord(const char* storageFileName);
	HRESULT StopRecord();

	IMediaEventEx* GetEventHandle();

private:
	HRESULT CheckMediaType(CComPtr<IEnumMediaTypes> pEnumMedia, GUID MediaType);
	HRESULT SaveGraphFile(CComPtr<IGraphBuilder> pGraph, WCHAR *wszPath);
	HRESULT ConnectFilters(CComPtr<IGraphBuilder> pGraph, CComPtr<IBaseFilter> pSrc, CComPtr<IBaseFilter> pDest, GUID MediaType);
	HRESULT GetUnconnectedPin(CComPtr<IBaseFilter> pFilter, PIN_DIRECTION PinDir, IPin **ppPin, GUID MediaType);
	HRESULT Init();

	CComPtr<IGraphBuilder> m_pGraphBuilder;
	CComPtr<IMediaControl> m_pMediaControl;
	CComPtr<IMediaEventEx> m_pMediaEvent;
	CComPtr<IMediaSeeking> m_pMeidaSeeking;
	CComPtr<IVideoWindow> m_pVideoWindow;
	CComPtr<IBasicVideo> m_pBasicVideo;
	CComPtr<IRecordStream> m_pRecordStream;
	CComPtr<ISetCallBack> m_pSetCallBack;

	BOOL m_bDisplay;
};

extern TMRECEIVERGRAPH_API int nTMReceiverGraph;

TMRECEIVERGRAPH_API int fnTMReceiverGraph(void);
