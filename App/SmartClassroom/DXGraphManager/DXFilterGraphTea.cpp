#include "stdafx.h"
#include "DXFilterGraphTea.h"
#include "Utils.h"
#include "ModuleFactory.h"



CDXFilterGraphTea::CDXFilterGraphTea()
{
	init();
	return;
}


HRESULT CDXFilterGraphTea::BuildGraph(BOOL bDisplay)
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
		hr = CUtils::AddFilter2(m_pGraphBuilder, CLSID_VideoInputDeviceCategory, L"TW6802 PCI, Analog 01 Capture", &pSrc);
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

		//hr = CUtils::SaveGraphFile(m_pGraphBuilder, L"F:\\TMReceiver.grf");
		syncConfiguration();
		return S_OK;
	}
}


HRESULT CDXFilterGraphTea::StartTracking(BOOL bShowTrackingRes)
{
	HRESULT hr = E_FAIL;
	if(m_pTrackingControl)
	{
		syncConfiguration();
		hr = m_pTrackingControl->StartTracking(bShowTrackingRes);
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

HRESULT CDXFilterGraphTea::EraseCachedVertexes()
{
	HRESULT hr = E_FAIL;
	if(m_pTrackingControl)
	{
		hr = m_pTrackingControl->EraseCachedVertexes();
	}
	return hr;
}

HRESULT CDXFilterGraphTea::CacheAndShowBZoneVertex( int xPix, int yPix )
{
	HRESULT hr = E_FAIL;
	if(m_pTrackingControl)
	{
		hr = m_pTrackingControl->CacheAndShowBZoneVertex(xPix, yPix);
	}
	return hr;
}

HRESULT CDXFilterGraphTea::AddBlindZone(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
	HRESULT hr = E_FAIL;
	if(m_pTrackingControl)
	{
		hr = m_pTrackingControl->AddBZone(x1, y1, x2, y2, x3, y3, x4, y4);
	}
	return hr;
}

HRESULT CDXFilterGraphTea::ClearBlindZones()
{
	HRESULT hr = E_FAIL;
	if(m_pTrackingControl)
	{
		hr = m_pTrackingControl->ClearBlindZones();
	}
	return hr;
}

HRESULT CDXFilterGraphTea::SetFullScreenStrategy(int fullScrMinDur, int noPersonMaxDur)
{
	HRESULT hr = E_FAIL;
	if(m_pTrackingControl)
	{
		hr = m_pTrackingControl->SetFullScrStrategy(fullScrMinDur, noPersonMaxDur);
	}
	return hr;
}


HRESULT CDXFilterGraphTea::syncConfiguration()
{
	HRESULT hr = E_FAIL;
	if(m_pTrackingControl)
	{
		CConfigManager *pConfigManager = ((CModuleFactory *)m_pModuleFactory)->GetConfigManager();
		CDXGraphManager *pDXGraphManager = ((CModuleFactory *)m_pModuleFactory)->GetGraphManager();

		int beginX = -1, beginY = -1, beginWidth = -1, beginHeight = -1, stopX = -1, stopY = -1, stopWidth = -1, stopHeight = -1;
		pConfigManager->GetTeaBeginTrackingArea(beginX, beginY, beginWidth, beginHeight);
		pConfigManager->GetTeaStopTrackingArea(stopX, stopY, stopWidth, stopHeight);
		m_pTrackingControl->ConfigTrackingArea(beginX, beginY, beginWidth, beginHeight, stopX, stopY, stopWidth, stopHeight);

		int leastHumanGap = -1, huamnWidth = -1;
		leastHumanGap = pConfigManager->GetTeaLeastHumanGap();
		huamnWidth = pConfigManager->GetTeaHumanWidth();
		m_pTrackingControl->ConfigHuman(leastHumanGap, huamnWidth);

		int disappearFrameThresh = -1, centerWeightThresh = -1, fgLowThresh = -1, fgUpThresh = -1;
		double fgHistThresh = -1.0;
		disappearFrameThresh = pConfigManager->GetTeaDisFrameThresh();
		centerWeightThresh = pConfigManager->GetTeaCenterWeightThresh();
		fgLowThresh = pConfigManager->GetTeaFgLowThresh();
		fgUpThresh = pConfigManager->GetTeaFgUpThresh();
		fgHistThresh = pConfigManager->GetTeaFgHistThresh();
		m_pTrackingControl->ConfigVariousThresh(disappearFrameThresh, centerWeightThresh, fgLowThresh, fgUpThresh, fgHistThresh);

		double gbmLearningRate = -1.0;
		int trackingInterval = -1;
		gbmLearningRate = pConfigManager->GetTeaGBMLearningRate();
		trackingInterval = pConfigManager->GetTeaTrackingInterval();
		m_pTrackingControl->ConfigMiscellaneous(gbmLearningRate, trackingInterval);

		BlindZoneList *pBZoneList = NULL;
		pConfigManager->GetBlindZoneList(&pBZoneList);
		if(pBZoneList != NULL)
		{
			BlindZoneIter bzoneIter;
			for(bzoneIter=pBZoneList->begin(); bzoneIter!=pBZoneList->end(); ++bzoneIter)
			{
				AddBlindZone(bzoneIter->x[0], bzoneIter->y[0],
					         bzoneIter->x[1], bzoneIter->y[1],
							 bzoneIter->x[2], bzoneIter->y[2],
							 bzoneIter->x[3], bzoneIter->y[3]);
			}
		}
		return S_OK;
	}
	return E_FAIL;
}


// 
// ------ Teacher PTZ Graph ------ 
// 
CDXFilterGraphTeaPTZ::CDXFilterGraphTeaPTZ()
{
	init();
	return;
}

HRESULT CDXFilterGraphTeaPTZ::BuildGraph(BOOL bDisplay)
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
		hr = CUtils::AddFilter2(m_pGraphBuilder, CLSID_VideoInputDeviceCategory, L"TW6802 PCI, Analog 02 Capture", &pSrc);
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

		hr = CUtils::ConnectFilters(m_pGraphBuilder, pSrc, pRenderer, MEDIATYPE_NULL);
		if(FAILED(hr)) return hr;

		//hr = CUtils::SaveGraphFile(m_pGraphBuilder, L"F:\\TMReceiver.grf");
		return S_OK;
	}
}
