// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the TMGRAPH_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TMGRAPH_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef TMGRAPH_EXPORTS
#define TMGRAPH_API __declspec(dllexport)
#else
#define TMGRAPH_API __declspec(dllimport)
#endif

#define SAFE_RELEASE(p)			{ if(p) { (p)->Release(); (p)=NULL; } }

#include "stdafx.h"
#include "DShow.h"
#include <initguid.h>
#include <atlcomcli.h>
#include "..\StreamReceiverSrc\TMReceiver.h"

// {345446A5-B638-40B4-BBE5-4098028AA14B}
DEFINE_GUID(CLSID_RGBSource32, 
	0x345446A5, 0xB638, 0x40B4, 0xBB, 0xE5, 0x40, 0x98, 0x02, 0x8A, 0xA1, 0x4B);

// {CEB13CC8-3591-45A5-BA0F-20E9A1D72F76}
DEFINE_GUID(CLSID_DecklinkRenderer, 
	0xceb13cc8, 0x3591, 0x45a5, 0xba, 0x0f, 0x20, 0xe9, 0xa1, 0xd7, 0x2f, 0x76);

// {C1F400A4-3F08-11D3-9F0B-006008039E37}
DEFINE_GUID(CLSID_NullRenderer, 
	0xc1f400a4, 0x3f08, 0x11d3, 0x9f, 0x0b, 0x00, 0x60, 0x08, 0x03, 0x9e, 0x37);

// {6F568873-134B-4FF3-B0B2-D886D859A0C7}
DEFINE_GUID(CLSID_StreamReceiver, 
	0x6f568873, 0x134b, 0x4ff3, 0xb0, 0xb2, 0xd8, 0x86, 0xd8, 0x59, 0xa0, 0xc7);

// {24D64F98-1D26-4D8D-88FA-FF996215B864}
DEFINE_GUID(IID_IRecordTMStream, 
	0x24d64f98, 0x1d26, 0x4d8d, 0x88, 0xfa, 0xff, 0x99, 0x62, 0x15, 0xb8, 0x64);
MIDL_INTERFACE("24D64F98-1D26-4D8D-88FA-FF996215B864")
IRecordTMStream : public IUnknown
{
	virtual HRESULT STDMETHODCALLTYPE StartRecord(const char* fileName)PURE;
	virtual HRESULT STDMETHODCALLTYPE StopRecord()PURE;
};

// {C456AF23-EF0E-4A49-A140-6AAC009975AF}
DEFINE_GUID(IID_ISetTMCallBack, 
	0xc456af23, 0xef0e, 0x4a49, 0xa1, 0x40, 0x6a, 0xac, 0x0, 0x99, 0x75, 0xaf);
MIDL_INTERFACE("C456AF23-EF0E-4A49-A140-6AAC009975AF")
ISetTMCallBack : public IUnknown
{
	virtual HRESULT STDMETHODCALLTYPE SetCallBackBeforeDecode(TMReceiverCB cb, void* arg)PURE;
	virtual HRESULT STDMETHODCALLTYPE SetCallBackAfterDecode(TMReceiverCB cb, void* arg)PURE;
};

#ifndef WM_GRAPHNOTIFY
#define WM_GRAPHNOTIFY (WM_APP + 100)
#endif

// This class is exported from the TMGraph.dll
class TMGRAPH_API CTMGraph {
public:
	CTMGraph(void);
	~CTMGraph();

	HRESULT Create();
	HRESULT BuildFilterGraph(const char* fileName, BOOL bDisplay);
	HRESULT SetDisplayWindow(HWND windowHandle);
	HRESULT SetNotifyWindow(HWND windowHandle);
	HRESULT SetBeforeDecodeCB(TMReceiverCB cb, void* arg);
	HRESULT SetAfterDecodeCB(TMReceiverCB cb, void* arg);
	void OnSize();

	HRESULT Stop();
	HRESULT Pause();
	HRESULT Run();

	HRESULT StartRecord(const char* storageFileName);
	HRESULT StopRecord();

	IMediaEventEx* GetEventHandle();

	HRESULT Destroy();

private:
	HRESULT CheckMediaType(CComPtr<IEnumMediaTypes> pEnumMedia, GUID MediaType);
	HRESULT SaveGraphFile(CComPtr<IGraphBuilder> pGraph, WCHAR *wszPath);
	HRESULT ConnectFilters(CComPtr<IGraphBuilder> pGraph, CComPtr<IBaseFilter> pSrc, CComPtr<IBaseFilter> pDest, GUID MediaType);
	HRESULT GetUnconnectedPin(CComPtr<IBaseFilter> pFilter, PIN_DIRECTION PinDir, IPin **ppPin, GUID MediaType);
	HRESULT Init();
	HRESULT AddFilter2(IGraphBuilder* pGraph, const GUID &clsid, LPCWSTR pName, IBaseFilter** ppFilter);

	CComPtr<IGraphBuilder> m_pGraphBuilder;
	CComPtr<IMediaControl> m_pMediaControl;
	CComPtr<IMediaEventEx> m_pMediaEvent;
	CComPtr<IMediaSeeking> m_pMeidaSeeking;
	CComPtr<IVideoWindow> m_pVideoWindow;
	CComPtr<IBasicVideo> m_pBasicVideo;
	CComPtr<IRecordTMStream> m_pRecordStream;
	CComPtr<ISetTMCallBack> m_pSetCallBack;

	BOOL m_bDisplay;
};

