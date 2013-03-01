// Analyzer.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Analyzer.h"
#include "ModuleFactory.h"
#include "CameraController.h"
#include "ConfigManager.h"

// This is the constructor of a class that has been exported.
// see Analyzer.h for the class definition
CPositionAnalyzer::CPositionAnalyzer()
{
	m_pModuleFactory = CModuleFactory::GetInstance();
	m_nPrevLocId = 0;
	return;
}


HRESULT CPositionAnalyzer::AnalyzeTeacherPositions(std::vector<Point2f> trackedPersons)
{
	int teaId = ((CModuleFactory *)m_pModuleFactory)->GetConfigManager()->GetTeaId();
	int teaFullScreenId = ((CModuleFactory *)m_pModuleFactory)->GetConfigManager()->GetTeaFullScreenLocId();
	if(trackedPersons.empty())
	{
		return S_FALSE;
	}
	else if(trackedPersons.size() > 1)
	{
		((CModuleFactory *)m_pModuleFactory)->GetCameraController()->RecallPreSetPos(teaId, teaFullScreenId);
		return S_OK;
	}
	else
	{
		PresetLocDict * teaPresetLocDict = NULL;
		PresetLocDictIter teaPresetLocDictIter;
		((CModuleFactory *)m_pModuleFactory)->GetConfigManager()->GetTeaPresetLocDict(&teaPresetLocDict);
		int rangeOverlap = ((CModuleFactory *)m_pModuleFactory)->GetConfigManager()->GetTeaPixRangeOverlap();
		for(teaPresetLocDictIter=teaPresetLocDict->begin(); teaPresetLocDictIter!=teaPresetLocDict->end(); ++teaPresetLocDictIter)
		{
			if(trackedPersons[0].x >= teaPresetLocDictIter->second.left && trackedPersons[0].x < teaPresetLocDictIter->second.right)
			{
				int locId = teaPresetLocDictIter->first;
				if(trackedPersons[0].x < teaPresetLocDictIter->second.left + rangeOverlap)
				{
					//Find the previous range
					PresetLocDictIter prev;
					for(prev=teaPresetLocDict->begin(); prev!=teaPresetLocDict->end(); ++prev)
					{
						if(teaPresetLocDictIter->second.left > prev->second.left &&
							teaPresetLocDictIter->second.left < prev->second.right)
						{
							locId = prev->first;
						}
					}
				}
				else if(trackedPersons[0].x >= teaPresetLocDictIter->second.right - rangeOverlap)
				{
					//Find the afterward range
					PresetLocDictIter after;
					for(after=teaPresetLocDict->begin(); after!=teaPresetLocDict->end(); ++after)
					{
						if(teaPresetLocDictIter->second.right > after->second.left &&
							teaPresetLocDictIter->second.right < after->second.right)
						{
							locId = after->first;
						}
					}
				}
				if(m_nPrevLocId != locId)
				{
					((CModuleFactory *)m_pModuleFactory)->GetCameraController()->RecallPreSetPos(teaId, locId);
					m_nPrevLocId = locId;
				}
				return S_OK;
			}
		}
		return S_FALSE;
	}
}