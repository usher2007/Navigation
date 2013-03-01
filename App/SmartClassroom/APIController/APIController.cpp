// APIController.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "APIController.h"



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

HRESULT CAPIController::BuildTeacherPTZGraph(BOOL bDisplay, HWND displayWnd, HWND notifyWnd)
{
	if(m_pModuleFactory)
	{
		m_pModuleFactory->GetGraphManager()->CreateTeacherPTZGraph(bDisplay, displayWnd, notifyWnd);
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CAPIController::addCamera( int cameraId, int comNum, int baudRate )
{
	if(m_pModuleFactory)
	{
		m_pModuleFactory->GetCameraController()->addCamera(cameraId, comNum, baudRate);
		restoreCameraPresetLoc(cameraId);
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CAPIController::restoreCameraPresetLoc(int cameraId)
{
	if(m_pModuleFactory)
	{
		PresetLocDict *pLocDict = NULL;
		m_pModuleFactory->GetConfigManager()->GetTeaPresetLocDict(&pLocDict);
		PresetLocDictIter locIter;
		for(locIter=pLocDict->begin(); locIter!=pLocDict->end(); ++locIter)
		{
			m_pModuleFactory->GetCameraController()->SetPreSetPos(cameraId, locIter->first, TRUE);
		}
		int fullScreenLocId = m_pModuleFactory->GetConfigManager()->GetTeaFullScreenLocId();
		m_pModuleFactory->GetCameraController()->SetPreSetPos(cameraId, fullScreenLocId, TRUE);
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CAPIController::AddTeaCamera()
{
	if(m_pModuleFactory)
	{
		int teaId = m_pModuleFactory->GetConfigManager()->GetTeaId();
		return addCamera(teaId, 1, 9600);
	}
	return E_FAIL;
}

HRESULT CAPIController::TeacherPTZUp()
{
	if(m_pModuleFactory)
	{
		int teaId = m_pModuleFactory->GetConfigManager()->GetTeaId();
		m_pModuleFactory->GetCameraController()->TurnUp(teaId);
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CAPIController::TeacherPTZDown()
{
	if(m_pModuleFactory)
	{
		int teaId = m_pModuleFactory->GetConfigManager()->GetTeaId();
		m_pModuleFactory->GetCameraController()->TurnDown(teaId);
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CAPIController::TeacherPTZLeft()
{
	if(m_pModuleFactory)
	{
		int teaId = m_pModuleFactory->GetConfigManager()->GetTeaId();
		m_pModuleFactory->GetCameraController()->TurnLeft(teaId);
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CAPIController::TeacherPTZRight()
{
	if(m_pModuleFactory)
	{
		int teaId = m_pModuleFactory->GetConfigManager()->GetTeaId();
		m_pModuleFactory->GetCameraController()->TurnRight(teaId);
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CAPIController::TeacherPTZZoomIn()
{
	if(m_pModuleFactory)
	{
		int teaId = m_pModuleFactory->GetConfigManager()->GetTeaId();
		m_pModuleFactory->GetCameraController()->ZoomIn(teaId);
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CAPIController::TeacherPTZZoomOut()
{
	if(m_pModuleFactory)
	{
		int teaId = m_pModuleFactory->GetConfigManager()->GetTeaId();
		m_pModuleFactory->GetCameraController()->ZoomOut(teaId);
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CAPIController::TeacherPTZStop()
{
	if(m_pModuleFactory)
	{
		int teaId = m_pModuleFactory->GetConfigManager()->GetTeaId();
		m_pModuleFactory->GetCameraController()->Stop(teaId);
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
		int teaId = m_pModuleFactory->GetConfigManager()->GetTeaId();
		m_pModuleFactory->GetCameraController()->SetPreSetPos(teaId, locId, FALSE);
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CAPIController::TeacherPTZRecallPrePos( int locId )
{
	if(m_pModuleFactory)
	{
		int teaId = m_pModuleFactory->GetConfigManager()->GetTeaId();
		m_pModuleFactory->GetCameraController()->RecallPreSetPos(teaId, locId);
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CAPIController::TeacherPTZGraphRun()
{
	HRESULT hr = E_FAIL;
	if(m_pModuleFactory)
	{
		hr = m_pModuleFactory->GetGraphManager()->RunTeacherPTZGraph();
	}
	return hr;
}

HRESULT CAPIController::TeacherPTZGraphStop()
{
	HRESULT hr = E_FAIL;
	if(m_pModuleFactory)
	{
		hr = m_pModuleFactory->GetGraphManager()->StopTeacherPTZGraph();
	}
	return hr;
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
		BOOL bShowTracking = m_pModuleFactory->GetConfigManager()->IsTeaShowTracking();
		hr = m_pModuleFactory->GetGraphManager()->StartTeacherTracking(bShowTracking);
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

HRESULT CAPIController::TeacherSetEnvParams(double roomWidth, double cameraDistance)
{
	HRESULT hr = E_FAIL;
	if(m_pModuleFactory)
	{
		hr = m_pModuleFactory->GetConfigManager()->SetTeaEnvParams(roomWidth, cameraDistance);
	}
	return hr;
}

HRESULT CAPIController::TeacherGetEnvParams(double& roomWidth, double& cameraDistance)
{
	HRESULT hr = E_FAIL;
	if(m_pModuleFactory)
	{
		hr = m_pModuleFactory->GetConfigManager()->GetTeaEnvParams(roomWidth, cameraDistance);
	}
	return hr;
}

HRESULT CAPIController::TeacherSetShowTracking(BOOL bShowTracking)
{
	HRESULT hr = E_FAIL;
	if(m_pModuleFactory)
	{
		hr = m_pModuleFactory->GetConfigManager()->SetTeaShowTracking(bShowTracking);
	}
	return hr;
}