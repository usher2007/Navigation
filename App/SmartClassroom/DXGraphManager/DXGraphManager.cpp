// DXGraphManager.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "DXGraphManager.h"


// This is the constructor of a class that has been exported.
// see DXGraphManager.h for the class definition
CDXGraphManager::CDXGraphManager()
{
	return;
}

HRESULT CDXGraphManager::CreateTeacherGraph( BOOL bDisplay, HWND displayWnd, HWND notifyWnd )
{
	HRESULT hr = S_FALSE;
	m_pTeacherGraph = new CDXFilterGraphTea();
	hr = m_pTeacherGraph->Create();
	if(FAILED(hr)) return hr;
	hr = m_pTeacherGraph->BuildGraph(bDisplay);
	if(FAILED(hr)) return hr;
	hr = m_pTeacherGraph->SetDisplayWindow(displayWnd);
	if(FAILED(hr)) return hr;
	hr = m_pTeacherGraph->SetNotifyWindow(notifyWnd);
	if(FAILED(hr)) return hr;
	return S_OK;
}

HRESULT CDXGraphManager::RunTeacherGraph()
{
	HRESULT hr = E_FAIL;
	if(m_pTeacherGraph)
	{
		hr = m_pTeacherGraph->Run();
	}
	return hr;
}

HRESULT CDXGraphManager::StopTeacherGraph()
{
	HRESULT hr = E_FAIL;
	if(m_pTeacherGraph)
	{
		hr = m_pTeacherGraph->Stop();
	}
	return hr;
}

HRESULT CDXGraphManager::StartTeacherTracking(BOOL bShowTrackingRes)
{
	HRESULT hr = E_FAIL;
	if(m_pTeacherGraph)
	{
		hr = m_pTeacherGraph->StartTracking(bShowTrackingRes);
	}
	return hr;
}

HRESULT CDXGraphManager::StopTeacherTracking()
{
	HRESULT hr = E_FAIL;
	if(m_pTeacherGraph)
	{
		hr = m_pTeacherGraph->StopTracking();
	}
	return hr;
}


HRESULT CDXGraphManager::CreateTeacherPTZGraph(BOOL bDisplay, HWND displayWnd, HWND notifyWnd)
{
	HRESULT hr = S_FALSE;
	m_pTeacherPTZGraph = new CDXFilterGraphTeaPTZ();
	hr = m_pTeacherPTZGraph->Create();
	if(FAILED(hr)) return hr;
	hr = m_pTeacherPTZGraph->BuildGraph(bDisplay);
	if(FAILED(hr)) return hr;
	hr = m_pTeacherPTZGraph->SetDisplayWindow(displayWnd);
	if(FAILED(hr)) return hr;
	hr = m_pTeacherPTZGraph->SetNotifyWindow(notifyWnd);
	if(FAILED(hr)) return hr;
	return S_OK;
}

HRESULT CDXGraphManager::RunTeacherPTZGraph()
{
	HRESULT hr = E_FAIL;
	if(m_pTeacherPTZGraph)
	{
		hr = m_pTeacherPTZGraph->Run();
	}
	return hr;
}

HRESULT CDXGraphManager::StopTeacherPTZGraph()
{
	HRESULT hr = E_FAIL;
	if(m_pTeacherPTZGraph)
	{
		hr = m_pTeacherPTZGraph->Stop();
	}
	return hr;
}