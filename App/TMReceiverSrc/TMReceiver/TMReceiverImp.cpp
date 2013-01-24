#include "stdafx.h"
#include "TMReceiverImp.h"

TMReceiverImp::TMReceiverImp()
{
	m_pGraph = new CTMReceiverGraph();
	m_bDisplay = FALSE;
	m_bStorage = FALSE;
	m_bRunning = FALSE;
	m_bStoring = FALSE;
	m_hDisplayWnd = NULL;
}

HRESULT TMReceiverImp::EnableDisplay(HWND hwnd)
{
	m_bDisplay = TRUE;
	m_hDisplayWnd = hwnd;
	return S_OK;
}

HRESULT TMReceiverImp::DisableDisplay()
{
	m_bDisplay = FALSE;
	if(m_pGraph != NULL)
	{
		HRESULT hr = m_pGraph->SetDisplayWindow(NULL);
		return hr;
	}
	return E_FAIL;
}

HRESULT TMReceiverImp::EnableStorage(const char* strFileName)
{
	memset(m_storageFileName, 0x00, sizeof(m_storageFileName));
	memcpy(m_storageFileName, strFileName, strlen(strFileName));
	m_bStorage = TRUE;
	return S_OK;
}

HRESULT TMReceiverImp::DisableStorage()
{
	m_bStorage = FALSE;
	return S_OK;
}

HRESULT TMReceiverImp::SetCallBackBeforeDecode(TMReceiverCB cb, void* arg)
{
	if(m_pGraph != NULL)
	{
		HRESULT hr = m_pGraph->SetBeforeDecodeCB(cb, arg);
	}
	return S_OK;
}

HRESULT TMReceiverImp::SetCallBackAfterDecode(TMReceiverCB cb, void* arg)
{
	if(m_pGraph != NULL)
	{
		HRESULT hr = m_pGraph->SetAfterDecodeCB(cb, arg);
	}
	return S_OK;
}

HRESULT TMReceiverImp::OpenRtspStream(const char* rtspUrl)
{
	return openStream(rtspUrl);
}

HRESULT TMReceiverImp::OpenHttpStream(const char* strFileName)
{
	return openStream(strFileName);
}

HRESULT TMReceiverImp::openStream(const char* streamName)
{
	if(m_pGraph != NULL)
	{
		m_pGraph->Create();
		HRESULT hr = m_pGraph->BuildFilterGraph(streamName, m_bDisplay);
		if(FAILED(hr)) return hr;
		if(!m_bDisplay)
		{
			hr = m_pGraph->Run();
			if(FAILED(hr)) return hr;
			m_bRunning = TRUE;
		}
		else
		{
			hr = m_pGraph->SetDisplayWindow(m_hDisplayWnd);
			if(FAILED(hr))
				return hr;
		}
		return S_OK;
	}
	return E_FAIL;
}

HRESULT TMReceiverImp::StartDisplay()
{
	if(m_bDisplay && m_pGraph != NULL)
	{
		HRESULT hr = m_pGraph->Run();
		if(FAILED(hr)) return hr;
		m_bRunning = TRUE;
		return S_OK;
	}
	return E_FAIL;
}

HRESULT TMReceiverImp::StopDisplay()
{
	if(m_bDisplay && m_pGraph != NULL)
	{
		HRESULT hr = m_pGraph->Stop();
		if(FAILED(hr)) return hr;
		m_bRunning = FALSE;
		return S_OK;
	}
	return E_FAIL;
}

HRESULT TMReceiverImp::StartStorage()
{
	if(m_bStorage && m_pGraph != NULL)
	{
		HRESULT hr = m_pGraph->StartRecord(m_storageFileName);
		if(FAILED(hr)) return hr;
		m_bStoring = TRUE;
		return S_OK;
	}
	return E_FAIL;
}

HRESULT TMReceiverImp::StopStorage()
{
	if(m_bStorage && m_pGraph != NULL)
	{
		HRESULT hr = m_pGraph->StopRecord();
		if(FAILED(hr)) return hr;
		m_bStoring = FALSE;
		return S_OK;
	}
	return E_FAIL;
}

HRESULT TMReceiverImp::Close()
{
	HRESULT hr = E_FAIL;
	if(m_pGraph != NULL)
	{
		if(m_bStoring)
		{
			hr = m_pGraph->StopRecord();
			if(FAILED(hr)) return hr;
			m_bStoring = FALSE;
		}
		
		if(m_bRunning)
		{
			hr = m_pGraph->Stop();
			if(FAILED(hr)) return hr;
			m_bRunning = FALSE;
		}
		//Destroy the graph?
		hr = m_pGraph->Destroy();
		return hr;
	}
	return E_FAIL;
}