// StandUpAnalyzer.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "StandUpAnalyzer.h"
#include "ModuleFactory.h"


CStandUpAnalyzer* CStandUpAnalyzer::m_pInstance = NULL;

const double    SLOPE_UP_THRESH       = 1.25;
const double    SLOPE_UP_THRESH_2     = 1.00;
const double    SLOPE_DOWN_THRESH     = -1.0;
const double    SLOPE_DOWN_THRESH_2   = -0.7;
const double    COL_THRESH_1          = 0.5;
const double    COL_THRESH_2          = 1.5;
const int       ROW_NUM               = 4;

CStandUpAnalyzer* CStandUpAnalyzer::GetInstance()
{
	if(m_pInstance != NULL)
	{
		return m_pInstance;
	}
	else
	{
		m_pInstance = new CStandUpAnalyzer;
		return m_pInstance;
	}
}
// This is the constructor of a class that has been exported.
// see StandUpAnalyzer.h for the class definition
CStandUpAnalyzer::CStandUpAnalyzer()
{
	m_StandUpInfo[0].clear();
	m_StandUpInfo[1].clear();
	m_pModulerFactory = CModuleFactory::GetInstance();
	for(int i=0; i<ROW_NUM; ++i)
	{
		m_standUpStatus[i] = 0;
	}
	return;
}

HRESULT CStandUpAnalyzer::AnalyzePosition( int index, StandUpRowInfo info )
{
	m_StandUpInfo[index] = info;
	controlPTZCamera();
	return S_OK;
}

HRESULT CStandUpAnalyzer::controlPTZCamera()
{

	int stuId = 1;
	if(m_StandUpInfo[0].empty() || m_StandUpInfo[1].empty())
	{
		return E_FAIL;
	}

	int rowNum = 0, colNum = 0;
	StandUpInfoIter infoIter;
	for(infoIter = m_StandUpInfo[0].begin(); infoIter != m_StandUpInfo[0].end(); ++infoIter)
	{
		StandUpInfoIter secondIter;
		if((secondIter = m_StandUpInfo[1].find(infoIter->first)) != m_StandUpInfo[1].end())
		{
			StandUpInfo s0 = infoIter->second;
			StandUpInfo s1 = secondIter->second;
			if(isStandUp(s0, s1)) // Stand Up
			{
				double ratio = s0.weight / s1.weight;
				if(ratio < COL_THRESH_1)
				{
					colNum = 1;
				}
				else if(ratio < COL_THRESH_2)
				{
					colNum = 2;
				}
				else
				{
					colNum = 3;
				}
				rowNum = infoIter->first;
				m_standUpStatus[rowNum] = 1;
			}
			else if(isSitDown(s0, s1)) // sit down
			{
				m_standUpStatus[rowNum] = 0;
			}
		}
		

	}

	int statusSum = 0;
	for(int i=0; i<ROW_NUM; ++i)
	{
		statusSum += m_standUpStatus[i];
	}
	if(statusSum == 1)
	{
		int cameraPosId = rowNum*3 + colNum + 1;   // 0 is for fullscreen.
		// Turn camera to specific position according to rowNum and colNum
		//((CModuleFactory *)m_pModulerFactory)->GetCameraController()->RecallPreSetPos(stuId, cameraPosId);
		char tmp[1024];
		memset(tmp, 0x00, 1024);
		sprintf(tmp, "FINAL RESULT: Row: %d, Col %d\n", rowNum, colNum);
		OutputDebugStringA(tmp);
	}
	else 
	{
		// Turn camera to full screen position.
		//((CModuleFactory *)m_pModulerFactory)->GetCameraController()->RecallPreSetPos(stuId, 0);
		char tmp[1024];
		memset(tmp, 0x00, 1024);
		sprintf(tmp, "FINAL RESULT: Full Screen StatusSum: %d\n", statusSum);
		OutputDebugStringA(tmp);
	}
}

bool CStandUpAnalyzer::isStandUp(StandUpInfo s0, StandUpInfo s1)
{
	if(s0.slope < SLOPE_UP_THRESH || s1.slope < SLOPE_UP_THRESH)
	{
		return false;
	}

	int count = 0;
	while(!s0.cachedSlope.empty())
	{
		if(s0.cachedSlope.front() > SLOPE_UP_THRESH_2)
		{
			count++;
		}
		s0.cachedSlope.pop();
	}
	if(count < 3)
	{
		return false;
	}

	count = 0;
	while(!s1.cachedSlope.empty())
	{
		if(s1.cachedSlope.front() > SLOPE_UP_THRESH_2)
		{
			count++;
		}
		s1.cachedSlope.pop();
	}
	if(count < 3)
	{
		return false;
	}

	return true;
}

bool CStandUpAnalyzer::isSitDown(StandUpInfo s0, StandUpInfo s1)
{
	if(s0.slope > SLOPE_DOWN_THRESH || s1.slope > SLOPE_DOWN_THRESH)
	{
		return false;
	}

	int count = 0;
	while(!s0.cachedSlope.empty())
	{
		if(s0.cachedSlope.front() < SLOPE_DOWN_THRESH_2)
		{
			count++;
		}
		s0.cachedSlope.pop();
	}
	if(count < 3)
	{
		return false;
	}

	count = 0;
	while(!s1.cachedSlope.empty())
	{
		if(s1.cachedSlope.front() < SLOPE_DOWN_THRESH_2)
		{
			count++;
		}
		s1.cachedSlope.pop();
	}
	if(count < 3)
	{
		return false;
	}

	return true;
}
