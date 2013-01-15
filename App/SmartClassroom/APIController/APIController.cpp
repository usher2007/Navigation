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

int CAPIController::TeacherPTZUp()
{
	if(m_pModuleFactory)
	{
		m_pModuleFactory->GetCameraController()->TurnUp(TEA_CAMERA_ID);
		return S_OK;
	}
	return E_FAIL;
}

int CAPIController::TeacherPTZDown()
{
	if(m_pModuleFactory)
	{
		m_pModuleFactory->GetCameraController()->TurnDown(TEA_CAMERA_ID);
		return S_OK;
	}
	return E_FAIL;
}

int CAPIController::TeacherPTZLeft()
{
	if(m_pModuleFactory)
	{
		m_pModuleFactory->GetCameraController()->TurnLeft(TEA_CAMERA_ID);
		return S_OK;
	}
	return E_FAIL;
}

int CAPIController::TeacherPTZRight()
{
	if(m_pModuleFactory)
	{
		m_pModuleFactory->GetCameraController()->TurnRight(TEA_CAMERA_ID);
		return S_OK;
	}
	return E_FAIL;
}

int CAPIController::TeacherPTZZoomIn()
{
	if(m_pModuleFactory)
	{
		m_pModuleFactory->GetCameraController()->ZoomIn(TEA_CAMERA_ID);
		return S_OK;
	}
	return E_FAIL;
}

int CAPIController::TeacherPTZZoomOut()
{
	if(m_pModuleFactory)
	{
		m_pModuleFactory->GetCameraController()->ZoomOut(TEA_CAMERA_ID);
		return S_OK;
	}
	return E_FAIL;
}

int CAPIController::TeacherPTZSetPrePos(int locId)
{
	if(m_pModuleFactory)
	{
		m_pModuleFactory->GetCameraController()->SetPreSetPos(TEA_CAMERA_ID, locId);
		return S_OK;
	}
	return E_FAIL;
}

int CAPIController::TeacherPTZRecallPrePos(int locId)
{
	if(m_pModuleFactory)
	{
		m_pModuleFactory->GetCameraController()->RecallPreSetPos(TEA_CAMERA_ID, locId);
		return S_OK;
	}
	return E_FAIL;
}
