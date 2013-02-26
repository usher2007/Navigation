// ConfigManager.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ConfigManager.h"


// This is the constructor of a class that has been exported.
// see ConfigManager.h for the class definition
CConfigManager::CConfigManager()
{
	m_teacherEnt.id = 0;
	m_teacherEnt.fullScreenLocId = 0;
	m_teacherEnt.pixRangeOverlap = 20;

	m_studentEnt.id = 1;
	m_laserPointEnt.id = 1;
	return;
}

HRESULT CConfigManager::SetTeacherPresetLoc( int locId, int leftRange, int rightRange )
{
	LocRange tmpLoc;
	tmpLoc.left = leftRange;
	tmpLoc.right = rightRange;
	m_teacherEnt.presetLocDict[locId] = tmpLoc;
	return S_OK;
}

HRESULT CConfigManager::SetTeacherFullScreen( int locId )
{
	m_teacherEnt.fullScreenLocId = locId;
	return S_OK;
}

HRESULT CConfigManager::SetTeaEnvParams( double roomWidth, double cameraDistance )
{
	m_teacherEnt.roomWidth = roomWidth;
	m_teacherEnt.cameraDistance = cameraDistance;
	return S_OK;
}

HRESULT CConfigManager::GetTeaEnvParams(double& roomWidth, double& cameraDistance)
{
	roomWidth = m_teacherEnt.roomWidth;
	cameraDistance = m_teacherEnt.cameraDistance;
	return S_OK;
}

HRESULT CConfigManager::GetTeaPresetLocDict(PresetLocDict& locDict)
{
	locDict = m_teacherEnt.presetLocDict;
	return S_OK;
}

int CConfigManager::GetTeaId()
{
	return m_teacherEnt.id;
}

int CConfigManager::GetTeaFullScreenLocId()
{
	return m_teacherEnt.fullScreenLocId;
}

int CConfigManager::GetTeaPixRangeOverlap()
{
	return m_teacherEnt.pixRangeOverlap;
}