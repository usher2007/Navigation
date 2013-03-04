// ConfigManager.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ConfigManager.h"
#include <fstream>

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
	std::ifstream teacherIn(TeacherConfigFile);
	std::string configLine, paramName, paramValue;
	while(!teacherIn.eof())
	{
		getline(teacherIn, configLine);
		getParamNameAndVal(configLine, paramName, paramValue);
		setTeaParametersFromFile(paramName, paramValue);
	}
	teacherIn.close();
	return S_OK;
}

HRESULT CConfigManager::getParamNameAndVal(const std::string& paramLine, std::string& paramName, std::string& paramValue)
{
	std::size_t nDelimiterPos = paramLine.find('=');
	paramName = paramLine.substr(0,nDelimiterPos);
	paramValue = paramLine.substr(nDelimiterPos+1, paramLine.length()-nDelimiterPos-1);
	return S_OK;
}

HRESULT CConfigManager::setTeaParametersFromFile( const std::string& paramName, std::string& paramValue )
{
	if(paramName.compare("ID") == 0)
	{
		m_teacherEnt.id = atoi(paramValue.c_str());
	}
	else if(paramName.compare("CLS_RM_WIDTH") == 0)
	{
		m_teacherEnt.roomWidth = atof(paramValue.c_str());
	}
	else if(paramName.compare("CAM_DIST") == 0)
	{
		m_teacherEnt.cameraDistance = atof(paramValue.c_str());
	}
	else if(paramName.compare("FULL_SCR_LOC_ID") == 0)
	{
		m_teacherEnt.fullScreenLocId = atoi(paramValue.c_str());
	}
	else if(paramName.compare("PIX_LOC_OVERLAP") == 0)
	{
		m_teacherEnt.pixRangeOverlap = atoi(paramValue.c_str());
	}
	else if(paramName.compare("SHOW_TRACK_RES") == 0)
	{
		m_teacherEnt.bShowTracking = (atoi(paramValue.c_str()) == 1);
	}
	else if(paramName.compare("PRESET_LOC_NUM") == 0)
	{
		m_teacherEnt.numOfPresetLoc = atoi(paramValue.c_str());
	}
	else if(paramName.compare("PRESET_LOC_IDS") == 0)
	{
		int nDelimPos = 0, i = 0;
		for(i=1; i<m_teacherEnt.numOfPresetLoc; ++i)
		{
			if(i!=1)
			{
				nDelimPos++;
			}
			int nextDelimPos = paramValue.find(',', nDelimPos);
			std::string id = paramValue.substr(nDelimPos, nextDelimPos-nDelimPos);
			m_teacherEnt.presetLocIds[i-1]=atoi(id.c_str());
			nDelimPos = nextDelimPos;
		}
		std::string id = paramValue.substr(nDelimPos+1, paramValue.length()-nDelimPos-1);
		m_teacherEnt.presetLocIds[i-1] = atoi(id.c_str());
	}
	else if(paramName.compare("PRESET_LOC_PIX_RANGES") == 0)
	{
		int nDelimiPos = 0, i = 0;
		for(i=1; i<m_teacherEnt.numOfPresetLoc; ++i)
		{
			if(i!=1)
			{
				nDelimiPos++;
			}
			int nextDelimPos = paramValue.find(';', nDelimiPos);
			std::string ranges = paramValue.substr(nDelimiPos, nextDelimPos-nDelimiPos);
			int rangeDelimPos = ranges.find(',');
			LocRange tmpRange;
			tmpRange.left = atoi(ranges.substr(0, rangeDelimPos).c_str());
			tmpRange.right = atoi(ranges.substr(rangeDelimPos+1, ranges.length()-rangeDelimPos-1).c_str());
			m_teacherEnt.presetLocDict[m_teacherEnt.presetLocIds[i-1]] = tmpRange;
			nDelimiPos = nextDelimPos;
		}
		std::string ranges = paramValue.substr(nDelimiPos+1, paramValue.length()-nDelimiPos-1);
		int rangeDelimPos = ranges.find(',');
		LocRange tmpRange;
		tmpRange.left = atoi(ranges.substr(0, rangeDelimPos).c_str());
		tmpRange.right = atoi(ranges.substr(rangeDelimPos+1, ranges.length()-rangeDelimPos-1).c_str());
		m_teacherEnt.presetLocDict[m_teacherEnt.presetLocIds[i-1]] = tmpRange;
	}
	else // No this parameter
	{
		return E_FAIL;
	}
	return S_OK;
}