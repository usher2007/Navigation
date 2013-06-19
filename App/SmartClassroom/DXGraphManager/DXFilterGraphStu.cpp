#include "stdafx.h"
#include "DXFilterGraphStu.h"
#include "Utils.h"
#include "ModuleFactory.h"

CDXFilterGraphStu::CDXFilterGraphStu(int posNum) : num(posNum)
{
	init();
	m_pStandUpControl = NULL;
	return;
}

HRESULT CDXFilterGraphStu::BuildGraph(BOOL bDisplay)
{
	HRESULT hr = S_FALSE;

	if(m_pGraphBuilder != NULL)
	{
		m_bDisplay = bDisplay;

		CComPtr<IBaseFilter> pSrc;
		/*hr = CoCreateInstance(CLSID_TWCapture01, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void **)&pSrc);
		if(FAILED(hr)) return hr;
		hr = m_pGraphBuilder->AddFilter(pSrc, L"TW01");
		if(FAILED(hr)) return hr;*/
		if(num == 1)
		{
			hr = CUtils::AddFilter2(m_pGraphBuilder, CLSID_VideoInputDeviceCategory, L"TW6802 PCI, Analog 03 Capture", &pSrc);
			if(FAILED(hr)) return hr;
		}
		else if(num == 2)
		{
			hr = CUtils::AddFilter2(m_pGraphBuilder, CLSID_VideoInputDeviceCategory, L"TW6802 PCI, Analog 04 Capture", &pSrc);
			if(FAILED(hr)) return hr;
		}
		else
		{
			return E_FAIL;
		}

		CComPtr<IBaseFilter> pStandUpDetectorFilter;
		hr = E_FAIL;
		if(num == 1)
		{
			hr = CoCreateInstance(CLSID_StandUpDetector, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void **)&pStandUpDetectorFilter);
			hr = pStandUpDetectorFilter->QueryInterface(IID_IStandUpControl, (void **)&m_pStandUpControl);
			if(FAILED(hr)) return hr;
		}
		else if(num == 2)
		{
			hr = CoCreateInstance(CLSID_StandUpDetector2, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void **)&pStandUpDetectorFilter);
		}
		if(FAILED(hr)) return hr;
		hr = m_pGraphBuilder->AddFilter(pStandUpDetectorFilter, L"StandUp Detector");
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

		hr = CUtils::ConnectFilters(m_pGraphBuilder, pSrc, pStandUpDetectorFilter, MEDIATYPE_NULL);
		if(FAILED(hr)) return hr;
		hr = CUtils::ConnectFilters(m_pGraphBuilder, pStandUpDetectorFilter, pRenderer, MEDIATYPE_NULL);
		if(FAILED(hr)) return hr;

		return S_OK;
	}
	return E_FAIL;
}


HRESULT CDXFilterGraphStu::SetStandUpParams(int leftBorder, int rightBorder, int totalRowNum, int totalColNum, int detectLine)
{
	if(m_pStandUpControl)
	{
		m_pStandUpControl->SetParams(leftBorder, rightBorder, totalRowNum, totalColNum, detectLine);
		return S_OK;
	}
	return E_FAIL;
}

CDXFilterGraphStuPTZ::CDXFilterGraphStuPTZ()
{
	init();
	return;
}

HRESULT CDXFilterGraphStuPTZ::BuildGraph(BOOL bDisplay)
{
	HRESULT hr = S_FALSE;

	if(m_pGraphBuilder != NULL)
	{
		m_bDisplay = bDisplay;

		CComPtr<IBaseFilter> pSrc;
		hr = CUtils::AddFilter2(m_pGraphBuilder, CLSID_VideoInputDeviceCategory, L"TW6802 PCI, Analog 05 Capture", &pSrc);
		if(FAILED(hr)) return hr;

		// TODO: need to add the detector filter.

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

		hr = CUtils::ConnectFilters(m_pGraphBuilder, pSrc, pRenderer, MEDIATYPE_NULL);
		if(FAILED(hr)) return hr;

		return S_OK;
	}
	return E_FAIL;
}