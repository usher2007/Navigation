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

HRESULT CAPIController::DumpConfiguration()
{
	if(m_pModuleFactory)
	{
		HRESULT hr = m_pModuleFactory->GetConfigManager()->DumpConfigFile();
		return hr;
	}
	return E_FAIL;
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

HRESULT CAPIController::TeacherSetDetailParams(int pixOverlap, double classroomWidth, double cameraDistance, int leastHumanGap, 
	                                           int humanWidth, int fgLowThresh, int fgUpThresh, double fgHistThresh)
{
	HRESULT hr = E_FAIL;
	if(m_pModuleFactory)
	{
		hr = m_pModuleFactory->GetConfigManager()->SetTeaDetailParams(pixOverlap, classroomWidth, cameraDistance, leastHumanGap, humanWidth, fgLowThresh, fgUpThresh, fgHistThresh);
	}
	return hr;
}

HRESULT CAPIController::TeacherSetTrackingArea(int beginX, int beginY, int beginW, int beginH, int stopX, int stopY, int stopW, int stopH)
{
	HRESULT hr = E_FAIL;
	if(m_pModuleFactory)
	{
		hr = m_pModuleFactory->GetConfigManager()->SetTeaTrackingArea(beginX, beginY, beginW, beginH, stopX, stopY, stopW, stopH);
	}
	return hr;
}

HRESULT CAPIController::TeacherSetCommonParams(int disappearFrameThresh, int centerWeightThresh, double gbmLearningRate, int trackingInterval)
{
	HRESULT hr = E_FAIL;
	if(m_pModuleFactory)
	{
		hr = m_pModuleFactory->GetConfigManager()->SetTeaCommonParams(disappearFrameThresh, centerWeightThresh, gbmLearningRate, trackingInterval);
	}
	return hr;
}

HRESULT CAPIController::TeacherCachePointToShow(int xPix, int yPix)
{
	HRESULT hr = E_FAIL;
	if(m_pModuleFactory)
	{
		hr = m_pModuleFactory->GetGraphManager()->CacheAndShowTeacherBZoneVertex(xPix, yPix);
	}
	return hr;
}

HRESULT CAPIController::TeacherEraseCurrentBlindZone()
{
	HRESULT hr = E_FAIL;
	if(m_pModuleFactory)
	{
		hr = m_pModuleFactory->GetGraphManager()->EraseCachedTeacherBZoneVertex();
	}
	return hr;
}

HRESULT CAPIController::TeacherSaveBlindZone(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
	HRESULT hr = E_FAIL;
	if(m_pModuleFactory)
	{
		hr = m_pModuleFactory->GetConfigManager()->SetBlindZone(x1, y1, x2, y2, x3, y3, x4, y4);
		if(FAILED(hr))
			return hr;
		hr = m_pModuleFactory->GetGraphManager()->AddTeacherBlindZone(x1, y1, x2, y2, x3, y3, x4, y4);
		if(FAILED(hr)) return hr;
		Sleep(1000);
		hr = m_pModuleFactory->GetGraphManager()->EraseCachedTeacherBZoneVertex();
	}
	return hr;
}

HRESULT CAPIController::TeacherClearBlindZones()
{
	HRESULT hr = E_FAIL;
	if(m_pModuleFactory)
	{
		hr = m_pModuleFactory->GetConfigManager()->ClearBlindZones();
		if(FAILED(hr)) return hr;
		hr = m_pModuleFactory->GetGraphManager()->ClearBlindZones();
	}
	return hr;
}