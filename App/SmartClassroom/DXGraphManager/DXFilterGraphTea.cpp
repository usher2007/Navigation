#include "stdafx.h"
#include "DXFilterGraphTea.h"
#include "Utils.h"

CDXFilterGraphTea::CDXFilterGraphTea()
{
	init();
	return;
}

HRESULT CDXFilterGraphTea::init()
{
	CoInitialize(NULL);
	m_pGraphBuilder = NULL;
	m_pMediaControl = NULL;
	m_pMediaEvent = NULL;
	m_pVideoWindow = NULL;
	m_pBasicVideo = NULL;
	m_hDisplayWnd = NULL;
	m_pTrackingControl = NULL;
	m_bDisplay = FALSE;
	return S_OK;
}

HRESULT CDXFilterGraphTea::Create()
{
	HRESULT hr = S_FALSE;

	m_pGraphBuilder = NULL;
	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&m_pGraphBuilder);
	if(FAILED(hr)) return hr;

	m_pMediaControl = NULL;
	hr = m_pGraphBuilder->QueryInterface(IID_IMediaControl, (void **)&m_pMediaControl);
	if(FAILED(hr)) return hr;

	m_pMediaEvent = NULL;
	hr = m_pGraphBuilder->QueryInterface(IID_IMediaEvent, (void **)&m_pMediaEvent);
	if(FAILED(hr)) return hr;

	m_pVideoWindow = NULL;
	hr = m_pGraphBuilder->QueryInterface(IID_IVideoWindow, (void **)&m_pVideoWindow);
	if(FAILED(hr)) return hr;

	m_pBasicVideo = NULL;
	hr = m_pGraphBuilder->QueryInterface(IID_IBasicVideo, (void **)&m_pBasicVideo);
	if(FAILED(hr)) return hr;

	return S_OK;
}

HRESULT CDXFilterGraphTea::BuildGraph(BOOL bDisplay)
{
	HRESULT hr = S_FALSE;
	if(m_pGraphBuilder != NULL)
	{
		m_bDisplay = bDisplay;

		CComPtr<IBaseFilter> pSrc;
		hr = CoCreateInstance(CLSID_TWCapture01, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void **)&pSrc);
		if(FAILED(hr)) return hr;
		hr = m_pGraphBuilder->AddFilter(pSrc, L"TW01");
		if(FAILED(hr)) return hr;

		CComPtr<IBaseFilter> pTrackingAlgFilter;
		hr = CoCreateInstance(CLSID_TrackingAlg, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void **)&pTrackingAlgFilter);
		if(FAILED(hr)) return hr;
		hr = pTrackingAlgFilter->QueryInterface(IID_ITrackingControl, (void **)&m_pTrackingControl);
		if(FAILED(hr)) return hr;
		hr = m_pGraphBuilder->AddFilter(pTrackingAlgFilter, L"Tracking Algorithm");
		if(FAILED(hr)) return hr;

		CComPtr<IBaseFilter> pRenderer;
		if(m_bDisplay)
		{
			hr = CoCreateInstance(CLSID_VideoRenderer, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void **)&pRenderer);
			if(FAILED(hr)) return hr;
		}
		else
		{
			hr = CoCreateInstance(CLSID_NullRenderer,NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void **)&pRenderer);
			if(FAILED(hr)) return hr;
		}
		hr = m_pGraphBuilder->AddFilter(pRenderer, L"Renderer");
		if(FAILED(hr)) return hr;

		hr = CUtils::ConnectFilters(m_pGraphBuilder, pSrc, pTrackingAlgFilter, MEDIATYPE_NULL);
		if(FAILED(hr)) return hr;

		hr = CUtils::ConnectFilters(m_pGraphBuilder, pTrackingAlgFilter, pRenderer, MEDIATYPE_NULL);
		if(FAILED(hr)) return hr;

		hr = CUtils::SaveGraphFile(m_pGraphBuilder, L"F:\\TMReceiver.grf");
		return S_OK;
	}
}

HRESULT CDXFilterGraphTea::SetDisplayWindow(HWND windowHandle)
{
	if(m_pVideoWindow)
	{
		m_pVideoWindow->put_Visible(OAFALSE);
		m_pVideoWindow->put_Owner((OAHWND)windowHandle);
		RECT windowRect;
		::GetClientRect(windowHandle, &windowRect);
		m_pVideoWindow->put_Left(0);
		m_pVideoWindow->put_Top(0);
		m_pVideoWindow->put_Width(windowRect.right - windowRect.left);
		m_pVideoWindow->put_Height(windowRect.bottom - windowRect.top);
		m_pVideoWindow->put_WindowStyle(WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS);
		m_pVideoWindow->put_MessageDrain((OAHWND)windowHandle);

		if(windowHandle != NULL)
		{
			m_pVideoWindow->put_Visible(OATRUE);
		}
		else
		{
			m_pVideoWindow->put_Visible(OAFALSE);
		}
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CDXFilterGraphTea::SetNotifyWindow(HWND windowHandle)
{
	HRESULT hr = E_FAIL;
	if(m_pMediaEvent != NULL)
	{
		hr = m_pMediaEvent->SetNotifyWindow((OAHWND)windowHandle, WM_GRAPHNOTIFY, 0);
		return hr;
	}
	return E_FAIL;
}

HRESULT CDXFilterGraphTea::Run()
{
	HRESULT hr = E_FAIL;
	if(m_pMediaControl != NULL)
	{
		hr = m_pMediaControl->Run();
		return hr;
	}
	return E_FAIL;
}

HRESULT CDXFilterGraphTea::Stop()
{
	HRESULT hr = E_FAIL;
	if(m_pMediaControl != NULL)
	{
		hr = m_pMediaControl->Stop();
		return hr;
	}
	return E_FAIL;
}

IMediaEventEx * CDXFilterGraphTea::GetEventHandle()
{
	return m_pMediaEvent;
}

HRESULT CDXFilterGraphTea::Destroy()
{
	HRESULT hr = S_OK;

	m_pMediaEvent = NULL;
	m_pVideoWindow = NULL;
	m_pBasicVideo = NULL;

	if(m_pGraphBuilder && m_pMediaControl)
	{
		m_pMediaControl->Stop();

		CComPtr<IEnumFilters> pEnum = NULL;
		hr = m_pGraphBuilder->EnumFilters(&pEnum);
		if(SUCCEEDED(hr))
		{
			IBaseFilter *pFilter = NULL;
			while(S_OK == pEnum->Next(1, &pFilter, NULL))
			{
				FILTER_INFO filterInfo;
				if(SUCCEEDED(pFilter->QueryFilterInfo(&filterInfo)))
				{
					SAFE_RELEASE(filterInfo.pGraph);
					CComPtr<IEnumPins> pIEnumPins = NULL;
					hr = pFilter->EnumPins(&pIEnumPins);
					if(SUCCEEDED(hr))
					{
						IPin *pIPin = NULL;
						while(S_OK == pIEnumPins->Next(1, &pIPin, NULL))
						{
							m_pGraphBuilder->Disconnect(pIPin);
							SAFE_RELEASE(pIPin);
						}
					}
				}
				SAFE_RELEASE(pFilter);
			}
		}
	}
	else
	{
		hr = S_FALSE;
	}
	return hr;
}

HRESULT CDXFilterGraphTea::StartTracking()
{
	HRESULT hr = E_FAIL;
	if(m_pTrackingControl)
	{
		hr = m_pTrackingControl->StartTracking();
	}
	return hr;
}

HRESULT CDXFilterGraphTea::StopTracking()
{
	HRESULT hr = E_FAIL;
	if(m_pTrackingControl)
	{
		hr = m_pTrackingControl->StopTracking();
	}
	return hr;
}