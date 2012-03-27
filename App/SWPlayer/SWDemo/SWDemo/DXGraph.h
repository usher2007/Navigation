#pragma once
#include <iostream>
#include "DShow.h"
#include "iRGBFilters.h"
#include <initguid.h>
#include <qnetwork.h>


//#include <Wmcodecdsp.h>
#define __IDxtCompositor_INTERFACE_DEFINED__

#define __IDxtAlphaSetter_INTERFACE_DEFINED__

#define __IDxtJpeg_INTERFACE_DEFINED__

#define __IDxtKey_INTERFACE_DEFINED__
#include "qedit.h"
#ifndef WM_GRAPHNOTIFY
#define WM_GRAPHNOTIFY (WM_APP + 100)
#endif
#ifndef OUR_GUID_ENTRY
#define OUR_GUID_ENTRY(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
	DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8);
#endif
// 4A69B442-28BE-4991-969C-B500ADF5D8A8
OUR_GUID_ENTRY(CLSID_MPEG4SDecoder,
	//0x4a69b442, 0x28be, 0x4991, 0x96, 0x9c, 0xb5, 0x00, 0xad, 0xf5, 0xd8, 0xa8)
	//0x04fe9017, 0xf873, 0x410e, 0x87, 0x1e, 0xab, 0x91, 0x66, 0x1a, 0x4e, 0xf7)
	0x008bac12, 0xfbaf, 0x497b, 0x96, 0x70, 0xbc, 0x6f, 0x6f, 0xba, 0xe2, 0xc4)
	//57F2DB8B-E6BB-4513-9D43-DCD2A6593125
	OUR_GUID_ENTRY(CLSID_MP3Decoder,
	//0x57f2db8b, 0xe6bb, 0x4513, 0x9d, 0x43, 0xdc, 0xd2, 0xa6, 0x59, 0x31, 0x25)
	//0xbbeea841, 0x0a63, 0x4f52, 0xa7, 0xab, 0xa9, 0xb3, 0xa8, 0x4e, 0xd3, 0x8a)
	0x38be3000, 0xdbf4, 0x11d0, 0x86, 0x0e, 0x00, 0xa0, 0x24, 0xcf, 0xef, 0x6d)
	OUR_GUID_ENTRY(CLSID_QpixelConverter,
	0x690fd747, 0x9e46, 0x4b00, 0x88, 0x87, 0x10, 0xb3, 0x21, 0x03, 0x2c, 0xeb)
	OUR_GUID_ENTRY(MEDIATYPE_NULL,
	0x00000000, 0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)
	DEFINE_GUID(CLSID_FFSrcFilter, 
	0xcae3a730, 0x8ad, 0x4cf1, 0xb2, 0xbf, 0xee, 0xbc, 0x65, 0xa8, 0xdd, 0x8f);
// {65A8C240-09AA-4660-9BE1-57C97465D778}
DEFINE_GUID(IID_IGrabberVideo, 
	0x65a8c240, 0x9aa, 0x4660, 0x9b, 0xe1, 0x57, 0xc9, 0x74, 0x65, 0xd7, 0x78);
class MyEdit;
MIDL_INTERFACE("65a8c240-09aa-4660-9be1-57c97465d778")
IGrabberVideo : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE SaveSpecifiedFragment(LONGLONG* start, LONGLONG* end, char* strPathName)=0;
};
DEFINE_GUID(IID_INetDvr, 
	0xe2208759, 0xa530, 0x4e69, 0x87, 0x16, 0xa7, 0x6e, 0xe5, 0xe2, 0xbf, 0x39);
// {E2208759-A530-4E69-8716-A76EE5E2BF39}
MIDL_INTERFACE("E2208759-A530-4E69-8716-A76EE5E2BF39")
INetDvr : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE SetRecordFile(char* strPathName)=0;
	virtual HRESULT STDMETHODCALLTYPE StartRecord()=0;
	virtual HRESULT STDMETHODCALLTYPE StopRecord()=0;
};


class CDXGraph
{
private:
	HWND owner;
	IGraphBuilder *pGraph;
	IMediaControl *pControl;
	IMediaEventEx *pEvent;
	IMediaSeeking *pSeeking;
	IVideoWindow *mVideoWindow;
	BOOL bFullScreen;
	ISampleGrabber *pGrabber;
	IGrabberVideo *pVideoGrabber;
	INetDvr *pNetDvr;
	IVideoFrameStep *pVideoFrameStep;
	IBasicVideo *pBasicVideo;
	REFTIME aveTimePerFrame;
	double duration;
	int grabCount;

public:
	CDXGraph(void);
	~CDXGraph(void);

	HRESULT Stop();
	HRESULT Pause();
	HRESULT SetNotifyWindow(HWND hwnd);
	HRESULT Create(HWND hwnd);
	HRESULT RenderFile(CString mSourceFile);
	HRESULT RenderAVIFile(CString mSourceFile);
	HRESULT SetDisplayWindow(HWND hwnd);
	IMediaEventEx* GetEventHandle();
	HRESULT SetCurrentPosition(double pos);
	HRESULT Run();
	HRESULT FullScreen(HWND hwnd);
	HRESULT GetCurrentPosition(double *pos);
	HRESULT GetDuration(double *duration);
	HRESULT GetCurrentFrameNum(int *frame);
	HRESULT GetLastFrameNum(int *frame);
	HRESULT GetAveTimePerFrame(double *aveTimePerFrame);
	HRESULT ConnectFilters(IGraphBuilder *pGraph, IBaseFilter *pSrc, IBaseFilter *pDest, GUID MediaType);
	HRESULT GetUnconnectedPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir, IPin **ppPin, GUID MediaType);
	BOOL IsFullScreen();
	HRESULT EscFullScreen(HWND hwnd);
	HRESULT Grab(CString strFilePath);
	HRESULT GrabVideo(LONGLONG *start, LONGLONG *end, CString strPathName);
	HRESULT StepForward(int step);
	HRESULT StepBackward(int step);
	HRESULT SetRate( double rate );
	void OnSize();
	HRESULT GetState(OAFilterState* pfs);
	HRESULT GetVideoSize(long* pWidth, long* pHeight);
	HRESULT SetRecordFile(char* strPathname);
	HRESULT StartRecord();
	HRESULT StopRecord();

private:
	HRESULT AddGrabber(IBaseFilter **pGrabberF);
	HRESULT CheckMediaType(IEnumMediaTypes *pEnumMedia, GUID MediaType);
	HRESULT SaveGraphFile(IGraphBuilder *pGraph, WCHAR *wszPath);

};

