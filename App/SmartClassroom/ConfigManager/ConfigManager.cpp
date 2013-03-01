// ConfigManager.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ConfigManager.h"


// This is the constructor of a class that has been exported.
// see ConfigManager.h for the class definition
CConfigManager::CConfigManager()
{
	LoadConfigFile();

	m_studentEnt.id = 1;
	m_laserPointEnt.id = 1;
	return;
}

HRESULT CConfigManager::LoadConfigFile()
{
	HRESULT hr = loadTeacherConfig();
	if(FAILED(hr))
	{

	}
	return S_OK;
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

HRESULT CConfigManager::SetTeaShowTracking(BOOL bShowTracking)
{
	m_teacherEnt.bShowTracking = bShowTracking;
	return S_OK;
}

HRESULT CConfigManager::GetTeaEnvParams(double& roomWidth, double& cameraDistance)
{
	roomWidth = m_teacherEnt.roomWidth;
	cameraDistance = m_teacherEnt.cameraDistance;
	return S_OK;
}

HRESULT CConfigManager::GetTeaPresetLocDict( PresetLocDict** locDict )
{
	//locDict.clear();
	/*PresetLocDictIter pIter = m_teacherEnt.presetLocDict.begin();
	for(; pIter!=m_teacherEnt.presetLocDict.end(); ++pIter)
	{
		locDict[pIter->first] = pIter->second;
	}*/
	*locDict = &m_teacherEnt.presetLocDict;
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

BOOL CConfigManager::IsTeaShowTracking()
{
	return m_teacherEnt.bShowTracking;
}

HRESULT CConfigManager::loadTeacherConfig()
{
	m_teacherEnt.id = 0;
	m_teacherEnt.fullScreenLocId = 0;
	m_teacherEnt.pixRangeOverlap = 20;
	m_teacherEnt.roomWidth = 6.0;
	m_teacherEnt.cameraDistance = 3.0;
	m_teacherEnt.bShowTracking = FALSE;

	LocRange tmpRange;
	tmpRange.left = 0;
	tmpRange.right = 252;
	m_teacherEnt.presetLocDict[1] = tmpRange;
	tmpRange.left = 190;
	tmpRange.right = 408;
	m_teacherEnt.presetLocDict[2] = tmpRange;
	tmpRange.left = 365;
	tmpRange.right = 720;
	m_teacherEnt.presetLocDict[3] = tmpRange;
	return S_OK;
}