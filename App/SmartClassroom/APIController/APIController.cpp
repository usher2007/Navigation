// APIController.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "APIController.h"

const int TEA_CAMERA_ID = 0;
const int STU_CAMERA_ID = 1;


// This is the constructor of a class that has been exported.
// see APIController.h for the class definition

CAPIController* CAPIController::m_pInstance = NULL;

CAPIController::CAPIController()
{
	m_pModuleFactory = CModuleFactory::GetInstance();
	return;
}

CAPIController* CAPIController::GetInstance()
{
	if(m_pInstance != NULL)
	{
		return m_pInstance;
	}
	else
	{
		m_pInstance = new CAPIController;
		return m_pInstance;
	}
}

HRESULT CAPIController::BuildTeacherGraph( BOOL bDisplay, HWND displayWnd, HWND notifyWnd )
{
	if(m_pModuleFactory)
	{
		m_pModuleFactory->GetGraphManager()->CreateTeacherGraph(bDisplay, displayWnd, notifyWnd);
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CAPIController::AddCamera( int cameraId, int comNum, int baudRate )
{
	if(m_pModuleFactory)
	{
		m_pModuleFactory->GetCameraController()->addCamera(cameraId, comNum, baudRate);
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CAPIController::TeacherPTZUp()
{
	if(m_pModuleFactory)
	{
		m_pModuleFactory->GetCameraController()->TurnUp(TEA_CAMERA_ID);
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CAPIController::TeacherPTZDown()
{
	if(m_pModuleFactory)
	{
		m_pModuleFactory->GetCameraController()->TurnDown(TEA_CAMERA_ID);
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CAPIController::TeacherPTZLeft()
{
	if(m_pModuleFactory)
	{
		m_pModuleFactory->GetCameraController()->TurnLeft(TEA_CAMERA_ID);
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CAPIController::TeacherPTZRight()
{
	if(m_pModuleFactory)
	{
		m_pModuleFactory->GetCameraController()->TurnRight(TEA_CAMERA_ID);
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CAPIController::TeacherPTZZoomIn()
{
	if(m_pModuleFactory)
	{
		m_pModuleFactory->GetCameraController()->ZoomIn(TEA_CAMERA_ID);
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CAPIController::TeacherPTZZoomOut()
{
	if(m_pModuleFactory)
	{
		m_pModuleFactory->GetCameraController()->ZoomOut(TEA_CAMERA_ID);
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CAPIController::TeacherPTZStop()
{
	if(m_pModuleFactory)
	{
		m_pModuleFactory->GetCameraController()->Stop(TEA_CAMERA_ID);
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CAPIController::TeacherPTZSetPrePos( int locId, int pixLeft, int pixRight, double realLeft, double realRight )
{
	if(m_pModuleFactory)
	{
		if(locId == 0)
		{
			m_pModuleFactory->GetConfigManager()->SetTeacherFullScreen(locId);
		}
		else
		{
			m_pModuleFactory->GetConfigManager()->SetTeacherPresetLoc(locId, pixLeft, pixRight);
		}
		m_pModuleFactory->GetCameraController()->SetPreSetPos(TEA_CAMERA_ID, locId);
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CAPIController::TeacherPTZRecallPrePos( int locId )
{
	if(m_pModuleFactory)
	{
		m_pModuleFactory->GetCameraController()->RecallPreSetPos(TEA_CAMERA_ID, locId);
		return S_OK;
	}
	return E_FAIL;
}


HRESULT CAPIController::TeacherGraphRun()
{
	HRESULT hr = E_FAIL;
	if(m_pModuleFactory)
	{
		hr = m_pModuleFactory->GetGraphManager()->RunTeacherGraph();
	}
	return hr;
}

HRESULT CAPIController::TeacherGraphStop()
{
	HRESULT hr = E_FAIL;
	if(m_pModuleFactory)
	{
		hr = m_pModuleFactory->GetGraphManager()->StopTeacherGraph();
	}
	return hr;
}

HRESULT CAPIController::TeacherTrackingStart()
{
	HRESULT hr = E_FAIL;
	if(m_pModuleFactory)
	{
		hr = m_pModuleFactory->GetGraphManager()->StartTeacherTracking();
	}
	return hr;
}

HRESULT CAPIController::TeacherTrackingStop()
{
	HRESULT hr = E_FAIL;
	if(m_pModuleFactory)
	{
		hr = m_pModuleFactory->GetGraphManager()->StopTeacherTracking();
	}
	return hr;
}