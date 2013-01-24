// Analyzer.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Analyzer.h"
#include "CameraController.h"
#include "ConfigManager.h"

// This is the constructor of a class that has been exported.
// see Analyzer.h for the class definition
CPositionAnalyzer::CPositionAnalyzer()
{
	return;
}


HRESULT CPositionAnalyzer::AnalyzeTeacherPositions(std::vector<Point2f> trackedPersons)
{
	if(trackedPersons.empty())
	{
		return S_FALSE;
	}
	else if(trackedPersons.size() > 1)
	{
		m_pModuleFactory->GetCameraController()->RecallPreSetPos(0, 0);
		return S_OK;
	}
	else
	{
		PresetLocDict teaPresetLocDict;
		PresetLocDictIter teaPresetLocDictIter;
		m_pModuleFactory->GetConfigManager()->GetTeaPresetLocDict(teaPresetLocDict);
		for(teaPresetLocDictIter=teaPresetLocDict.begin(); teaPresetLocDictIter!=teaPresetLocDict.end(); ++teaPresetLocDictIter)
		{
			if(trackedPersons[0].x >= teaPresetLocDictIter->second.left && trackedPersons[0].y < teaPresetLocDictIter->second.right)
			{
				m_pModuleFactory->GetCameraController()->RecallPreSetPos(0, teaPresetLocDictIter->first);
				return S_OK;
			}
		}
		return S_FALSE;
	}
}