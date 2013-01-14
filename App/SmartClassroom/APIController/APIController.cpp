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
