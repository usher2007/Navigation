#pragma once
#include "DShow.h"
#include <InitGuid.h>


class CDXFilterGraphTea
{
public:
	CDXFilterGraphTea();

	HRESULT Create();
	HRESULT BuildGraph(BOOL bDisplay);
	HRESULT SetDisplayWindow(HWND windowHandle);
	HRESULT SetNotifyWindow(HWND windowHandle);
	

	HRESULT Run();
	HRESULT Stop();

	IMediaEventEx * GetEventHandle();

	HRESULT Destroy();

private:
	HRESULT init();

	CComPtr<IGraphBuilder> m_pGraphBuilder;
	CComPtr<IMediaControl> m_pMediaControl;
	CComPtr<IMediaEventEx> m_pMediaEvent;
	CComPtr<IVideoWindow> m_pVideoWindow;
	CComPtr<IBasicVideo> m_pBasicVideo;

	HWND m_hDisplayWnd;
	BOOL m_bDisplay;
};